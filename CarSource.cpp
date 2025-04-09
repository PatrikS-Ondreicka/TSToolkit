// Fill out your copyright notice in the Description page of Project Settings.

#include <numeric>
#include "CarSource.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "CarPath.h"
#include "CarSpawnController.h"
#include "Car.h"

/**
 * Constructor for ACarSource.
 * Sets default values for the car source's components and initializes the actor.
 */
ACarSource::ACarSource()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up components
	SourceBoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Root Box Component"));
	if (!SourceBoxRoot)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SourceBoxRoot in ACarSource constructor."));
	}
	SetRootComponent(SourceBoxRoot);

	SourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Source Static Mesh"));
	if (!SourceMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SourceMesh in ACarSource constructor."));
	}
	SourceMesh->SetupAttachment(RootComponent);

	SpawnCheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Spawn Check Box Component"));
	if (!SpawnCheckBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SpawnCheckBox in ACarSource constructor."));
	}
	SpawnCheckBox->SetupAttachment(RootComponent);

	// Set up event methods
	if (SpawnCheckBox)
	{
		SpawnCheckBox->SetCollisionProfileName("OverlapAll");
		SpawnCheckBox->SetGenerateOverlapEvents(true);
	}
}

/**
 * Called when the game starts or when the actor is spawned.
 * Sets up overlap event bindings and initializes paths.
 */
void ACarSource::BeginPlay()
{
	Super::BeginPlay();

	if (!SpawnCheckBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnCheckBox is null in BeginPlay."));
		return;
	}

	// Set up spawn check delegates
	SpawnCheckBox->OnComponentBeginOverlap.AddDynamic(this, &ACarSource::_OnSpawnCheckBeginOverlap);
	SpawnCheckBox->OnComponentEndOverlap.AddDynamic(this, &ACarSource::_OnSpawnCheckEndOverlap);

	// Initialize car paths
	_InitPath();
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACarSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Called when the actor is constructed or properties are changed in the editor.
 * Updates the spawn check box's position relative to the actor.
 *
 * @param Transform The transform of the actor.
 */
void ACarSource::OnConstruction(const FTransform& Transform)
{
	if (!SpawnCheckBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnCheckBox is null in OnConstruction."));
		return;
	}

	FRotator actorRotator = GetActorRotation();
	FVector worldOffset = SpawnCheckBox->GetComponentLocation() - GetActorLocation();
	FVector localOffset = GetActorTransform().InverseTransformVector(worldOffset);
	FVector newPosition = GetActorLocation() + GetActorRotation().RotateVector(localOffset);
	SpawnCheckBox->SetWorldLocation(newPosition);
}

/**
 * Spawns a car using the default car class.
 */
void ACarSource::SpawnDefaultCar()
{
	SpawnCar(DefaultCarClass);
}

/**
 * Spawns a car using the specified car class.
 *
 * @param CarClass The class of the car to spawn.
 */
void ACarSource::SpawnCar(TSubclassOf<ACar> CarClass)
{
	if (!_CanSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot spawn car because _CanSpawn is false."));
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in SpawnCar."));
		return;
	}

	if (!CarClass)
	{
		UE_LOG(LogTemp, Error, TEXT("CarClass is null in SpawnCar."));
		return;
	}

	ACarPath* selectedPath = _SelectPath();
	if (!selectedPath)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to select path for car spawn in SpawnCar."));
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Set up car spawn location and target
	int lastNodeIndex = selectedPath->Path->GetNumberOfSplinePoints() - 1;
	FVector carTargetLocation = selectedPath->Path->GetLocationAtSplinePoint(lastNodeIndex, ESplineCoordinateSpace::World);
	FVector carSpawnLocation = SpawnCheckBox->GetComponentLocation();
	FRotator carSpawnRotation = SpawnCheckBox->GetComponentRotation();

	// Spawn the car
	ACar* spawnedCar = world->SpawnActor<ACar>(CarClass, carSpawnLocation, carSpawnRotation, spawnParams);
	if (!spawnedCar)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn car in SpawnCar."));
		return;
	}

	// Initialize car properties
	spawnedCar->SetDestination(carTargetLocation);
	spawnedCar->SetPath(selectedPath);
	spawnedCar->StaticSpeed = CarStaticSpeed;
	if (IsNight)
	{
		spawnedCar->TurnLightsOn();
	}
	else
	{
		spawnedCar->TurnLightsOff();
	}

	// Initialize distance along spline
	float initDistance = selectedPath->Path->GetDistanceAlongSplineAtLocation(carSpawnLocation, ESplineCoordinateSpace::World);
	spawnedCar->SetInitDistanceAlongSpline(initDistance);
}

/**
 * Handles the event when another actor begins overlapping with the spawn check box.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The other actor involved in the overlap.
 * @param OtherComp The other component involved in the overlap.
 * @param OtherBodyIndex The body index of the other component.
 * @param bFromSweep Whether the overlap was caused by a sweep.
 * @param SweepResult The result of the sweep.
 */
void ACarSource::_OnSpawnCheckBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("_OnSpawnCheckBeginOverlap called with a null OtherActor."));
		return;
	}

	ACar* other = Cast<ACar>(OtherActor);
	if (other)
	{
		_CanSpawn = false;
	}
}

/**
 * Handles the event when another actor ends overlapping with the spawn check box.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The other actor involved in the overlap.
 * @param OtherComp The other component involved in the overlap.
 * @param OtherBodyIndex The body index of the other component.
 */
void ACarSource::_OnSpawnCheckEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("_OnSpawnCheckEndOverlap called with a null OtherActor."));
		return;
	}

	ACar* other = Cast<ACar>(OtherActor);
	if (other)
	{
		_CanSpawn = true;
	}
}

/**
 * Selects a path for a spawned car to follow based on probabilities.
 *
 * @return A pointer to the selected car path.
 */
ACarPath* ACarSource::_SelectPath()
{
	float randNum = FMath::RandRange(0.0f, 1.0f);
	float cumulative = 0.0f;

	for (ACarPath* path : Paths)
	{
		if (!path)
		{
			UE_LOG(LogTemp, Warning, TEXT("_SelectPath encountered a null path in Paths."));
			continue;
		}

		cumulative += path->Probability;
		if (randNum <= cumulative)
		{
			return path;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("_SelectPath failed to select a path."));
	return nullptr;
}

/**
 * Initializes the paths for the car source by sorting them based on probabilities.
 */
void ACarSource::_InitPath()
{
	if (Paths.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("_InitPath - no path assignet to the source"));
		return;
	}

	if (FMath::Abs(std::accumulate(Paths.begin(), Paths.end(), 0.0f,
		[](float current, ACarPath* path) {return current + path->Probability; }) - 1.0f) > KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning! Sum of probabilities of car paths is not equal to 1.0."));
	}

	Paths.Sort([](const ACarPath& left, const ACarPath& right)
		{
			return left.Probability < right.Probability;
		});
}

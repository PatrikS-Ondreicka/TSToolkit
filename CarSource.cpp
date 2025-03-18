// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSource.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "CarPath.h"
#include "CarSpawnController.h"
#include "Car.h"

#define MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(str));
#define ERROR_MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(str));

// Sets default values
ACarSource::ACarSource()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up of components
	SourceBoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Root Box Component"));
	SetRootComponent(SourceBoxRoot);

	SourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Source Static Mesh"));
	SourceMesh->SetupAttachment(RootComponent);

	SpawnCheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Spawn Check Box Component"));
	SpawnCheckBox->SetupAttachment(RootComponent);

	// Set up of event methods
	SpawnCheckBox->SetCollisionProfileName("OverlapAll");
	SpawnCheckBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ACarSource::BeginPlay()
{
	Super::BeginPlay();

	// Set up of spawn check delegates
	SpawnCheckBox->OnComponentBeginOverlap.AddDynamic(this, &ACarSource::_OnSpawnCheckBeginOverlap);
	SpawnCheckBox->OnComponentEndOverlap.AddDynamic(this, &ACarSource::_OnSpawnCheckEndOverlap);

	// Sort Car paths by probability
	_InitPath();
}

// Called every frame
void ACarSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarSource::OnConstruction(const FTransform& Transform)
{
	FRotator actorRotator = GetActorRotation();
	FVector worldOffset = SpawnCheckBox->GetComponentLocation() - GetActorLocation();
	FVector localOffset = GetActorTransform().InverseTransformVector(worldOffset);
	FVector newPosition = GetActorLocation() + GetActorRotation().RotateVector(localOffset);
	SpawnCheckBox->SetWorldLocation(newPosition);
}

void ACarSource::SpawnDefaultCar()
{
	SpawnCar(DefaultCarClass);
}

void ACarSource::SpawnCar(TSubclassOf<ACar> CarClass)
{
	if (!_CanSpawn)
	{
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		ERROR_MSG("World is null!");
		return;
	}

	if (!CarClass)
	{
		ERROR_MSG("Car Class is null!");
		return;
	}

	ACarPath* selectedPath = _SelectPath();

	if (selectedPath == nullptr)
	{
		ERROR_MSG("Unable to select path for car spawn");
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Set up of location of a car sink
	int lastNodeIndex = selectedPath->Path->GetNumberOfSplinePoints() - 1;
	FVector carTargetLocation = selectedPath->Path->GetLocationAtSplinePoint(lastNodeIndex, ESplineCoordinateSpace::World);
	FVector carSpawnLocation = SpawnCheckBox->GetComponentLocation();
	FRotator carSpawnRotation = SpawnCheckBox->GetComponentRotation();

	// Set up of car spawning
	float distance = (RootComponent->GetComponentLocation() - SpawnCheckBox->GetComponentLocation()).Size();
	FVector carSpawLocation = selectedPath->Path->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);

	auto spawnedCar = world->SpawnActor<ACar>(CarClass, carSpawnLocation, carSpawnRotation, spawnParams);
	if (!spawnedCar)
	{
		ERROR_MSG("Failed to spawn car!");
		return;
	}

	// Car defaults init
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

	// Init distance along spline
	float initDistance = selectedPath->Path->GetDistanceAlongSplineAtLocation(carSpawLocation, ESplineCoordinateSpace::World);
	spawnedCar->SetInitDistanceAlongSpline(initDistance);
}

void ACarSource::_OnSpawnCheckBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACar* other = Cast<ACar>(OtherActor);
	if (other != nullptr)
	{
		_CanSpawn = false;
	}
}

void ACarSource::_OnSpawnCheckEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ACar* other = Cast<ACar>(OtherActor);
	if (other != nullptr)
	{
		_CanSpawn = true;
	}
}

ACarPath* ACarSource::_SelectPath()
{
	float randNum = FMath::RandRange(0.0f, 1.0f);
	float cumulative = 0.0f;
	for (auto path : Paths)
	{
		cumulative += path->Probability;
		if (randNum <= cumulative)
		{
			return path;
		}
	}
	return nullptr;
}

void ACarSource::_InitPath()
{
	if (Paths.Num() < 2)
	{
		return;
	}

	float cumulative = 0.0f;
	for (auto path : Paths)
	{
		cumulative += path->Probability;
	}

	if (cumulative != 1.0f)
	{
		MSG("Warning! Sum of probabilities of car paths is not equal to 1.0");
	}

	Algo::Sort(Paths,
		[](ACarPath* left, ACarPath* right)
		{
			return left->Probability < right->Probability;
		});
}
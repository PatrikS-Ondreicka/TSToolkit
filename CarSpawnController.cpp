// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSpawnController.h"
#include "CarSource.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Define paths to car blueprints
TArray<FString> ACarSpawnController::CarBpPaths
{
#ifndef NO_LARGE_VEHICLES
	// Bus
	TEXT("/Game/BP/Cars/BP_Bus.BP_Bus_C"),
	// Delivery
	TEXT("/Game/BP/Cars/BP_Delivery.BP_Delivery_C"),
#endif
	// Coupe
	TEXT("/Game/BP/Cars/Coupe/BP_Coupe.BP_Coupe_C"),
	TEXT("/Game/BP/Cars/Coupe/BP_RedCoupe.BP_RedCoupe_C"),
	TEXT("/Game/BP/Cars/Coupe/BP_GreenCoupe.BP_GreenCoupe_C"),
	TEXT("/Game/BP/Cars/Coupe/BP_BlackCoupe.BP_BlackCoupe_C"),
	// Offroad
	TEXT("/Game/BP/Cars/Offroad/BP_Offroad.BP_Offroad_C"),
	TEXT("/Game/BP/Cars/Offroad/BP_RedOffroad.BP_RedOffroad_C"),
	TEXT("/Game/BP/Cars/Offroad/BP_YellowOffroad.BP_YellowOffroad_C"),
	TEXT("/Game/BP/Cars/Offroad/BP_BlackOffroad.BP_BlackOffroad_C"),
	// Suv
	TEXT("/Game/BP/Cars/Suv/BP_Suv.BP_Suv_C"),
	TEXT("/Game/BP/Cars/Suv/BP_RedSuv.BP_RedSuv_C"),
	TEXT("/Game/BP/Cars/Suv/BP_BrownSuv.BP_BrownSuv_C"),
	TEXT("/Game/BP/Cars/Suv/BP_BlackSuv.BP_BlackSuv_C"),
	// Van
	TEXT("/Game/BP/Cars/Van/BP_Van.BP_Van_C"),
	TEXT("/Game/BP/Cars/Van/BP_YellowVan.BP_YellowVan_C"),
	TEXT("/Game/BP/Cars/Van/BP_WhiteVan.BP_WhiteVan_C")
};

/**
 * Constructor for ACarSpawnController.
 * Initializes the car blueprint pool by loading classes from predefined paths.
 */
ACarSpawnController::ACarSpawnController()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (const FString& path : CarBpPaths)
	{
		ConstructorHelpers::FClassFinder<ACar> classFinder(*path);
		if (classFinder.Class)
		{
			CarBpPool.Add(classFinder.Class);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load car blueprint at path: %s"), *path);
		}
	}
}

/**
 * Called when the game starts or when the actor is spawned.
 * Registers all car sources and sets up the initial round.
 */
void ACarSpawnController::BeginPlay()
{
	Super::BeginPlay();

	if (bRegisterAllAtBeginPlay)
	{
		_RegisterAllSources();
	}

	if (Sources.Num() > 0)
	{
		_RoundSetUp();
	}

	SetNight(_IsNight);
}

/**
 * Sets up the round by initializing the spawn timer.
 */
void ACarSpawnController::_RoundSetUp()
{
	_TimerRunOut = false;

	if (SpawnRate <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnRate is invalid or zero. Timer will not be set."));
		return;
	}

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACarSpawnController::_TimerAction, SpawnRate, false);
}

/**
 * Called when the spawn timer runs out.
 * Sets the _TimerRunOut flag to true.
 */
void ACarSpawnController::_TimerAction()
{
	_TimerRunOut = true;
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Sets whether it is night time in the simulation.
 * Updates all car sources to reflect the night state.
 *
 * @param State True to set night time, false to set day time.
 */
void ACarSpawnController::SetNight(bool State)
{
	_IsNight = State;

	for (ACarSource* source : Sources)
	{
		if (!source)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetNight encountered a null source in Sources."));
			continue;
		}

		source->IsNight = State;
	}
}

/**
 * Registers all car sources in the simulation by finding all ACarSource actors in the world.
 */
void ACarSpawnController::_RegisterAllSources()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _RegisterAllSources."));
		return;
	}

	Sources.Empty();

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(world, ACarSource::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		ACarSource* source = Cast<ACarSource>(actor);
		if (source)
		{
			Sources.Add(source);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("_RegisterAllSources encountered a non-ACarSource actor."));
		}
	}
}

/**
 * Checks whether all car sources are ready to spawn cars.
 *
 * @return True if all sources can spawn cars, false otherwise.
 */
bool ACarSpawnController::_CanSourcesSpawn()
{
	if (Sources.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("_CanSourcesSpawn called but Sources is empty."));
		return false;
	}

	for (ACarSource* carSource : Sources)
	{
		if (!carSource)
		{
			UE_LOG(LogTemp, Warning, TEXT("_CanSourcesSpawn encountered a null source in Sources."));
			continue;
		}

		if (!carSource->GetCanSpawn())
		{
			return false;
		}
	}

	return true;
}

/**
 * Retrieves a random car class from the blueprint pool.
 *
 * @return A random TSubclassOf<ACar> from the CarBpPool, or nullptr if the pool is empty.
 */
TSubclassOf<ACar> ACarSpawnController::_GetRandomCarClass()
{
	if (CarBpPool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("_GetRandomCarClass called but CarBpPool is empty."));
		return nullptr;
	}

	int randomIndex = UKismetMathLibrary::RandomInteger(CarBpPool.Num());

	if (CarBpPool.IsValidIndex(randomIndex))
	{
		return CarBpPool[randomIndex];
	}

	UE_LOG(LogTemp, Warning, TEXT("_GetRandomCarClass failed to retrieve a valid car class."));
	return nullptr;
}

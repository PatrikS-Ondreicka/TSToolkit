// Fill out your copyright notice in the Description page of Project Settings.

#include "PeriodicCarSpawnController.h"
#include "CarSource.h"

/**
 * Constructor for APeriodicCarSpawnController.
 * Initializes the periodic car spawn controller.
 */
APeriodicCarSpawnController::APeriodicCarSpawnController()
	: Super()
{
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic specific to periodic car spawning.
 */
void APeriodicCarSpawnController::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame to update the actor.
 * Handles periodic car spawning logic.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void APeriodicCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the timer has run out
	if (!_TimerRunOut)
	{
		return;
	}

	// Spawn cars at all sources and reset the round
	_SpawnAtAllSources();
	_RoundSetUp();
}

/**
 * Spawns cars at all registered car sources.
 * This method is called periodically to handle car spawning.
 */
void APeriodicCarSpawnController::_SpawnAtAllSources()
{
	if (Sources.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("_SpawnAtAllSources called but no sources are registered."));
		return;
	}

	for (ACarSource* CarSource : Sources)
	{
		if (!CarSource)
		{
			UE_LOG(LogTemp, Warning, TEXT("_SpawnAtAllSources encountered a null CarSource in Sources."));
			continue;
		}

		TSubclassOf<ACar> CarClass = _GetRandomCarClass();
		if (!CarClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("_SpawnAtAllSources failed to get a valid car class."));
			continue;
		}

		CarSource->SpawnCar(CarClass);
	}
}

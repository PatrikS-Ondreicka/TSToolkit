// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomCarSpawnController.h"
#include "CarSource.h"
#include "Engine/Engine.h"

/**
 * Called every frame to update the actor.
 * Handles random car spawning logic.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ARandomCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the timer has run out
	if (!_TimerRunOut)
	{
		return;
	}

	// Spawn a car at a random source and reset the round
	_SpawnCar();
	_RoundSetUp();
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic specific to random car spawning.
 */
void ARandomCarSpawnController::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Retrieves a random car source from the list of registered sources.
 *
 * @return A pointer to a randomly selected ACarSource, or nullptr if no sources are available.
 */
ACarSource* ARandomCarSpawnController::_GetRandomSource()
{
	if (Sources.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("_GetRandomSource called but no sources are registered."));
		return nullptr;
	}

	int index = FMath::RandRange(0, Sources.Num() - 1);
	ACarSource* randomSource = Sources[index];
	if (!randomSource)
	{
		UE_LOG(LogTemp, Warning, TEXT("_GetRandomSource selected a null source."));
	}
	return randomSource;
}

/**
 * Spawns a car at a randomly selected source.
 * If the selected source cannot spawn a car, logs a warning.
 */
void ARandomCarSpawnController::_SpawnCar()
{
	ACarSource* source = _GetRandomSource();
	if (!source)
	{
		UE_LOG(LogTemp, Warning, TEXT("_SpawnCar failed to get a valid source."));
		return;
	}

	if (source->GetCanSpawn())
	{
		TSubclassOf<ACar> carClass = _GetRandomCarClass();
		if (!carClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("_SpawnCar failed to get a valid car class."));
			return;
		}

		source->SpawnCar(carClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("_SpawnCar: Selected source cannot spawn a car."));
	}
}

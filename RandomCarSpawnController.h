// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarSpawnController.h"
#include "RandomCarSpawnController.generated.h"

/**
 * ARandomCarSpawnController is a specialized car spawn controller that spawns cars
 * at random sources. It extends the functionality of ACarSpawnController.
 */
UCLASS()
class TSTOOLKIT_API ARandomCarSpawnController : public ACarSpawnController
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ARandomCarSpawnController.
	 * Initializes the random car spawn controller.
	 */
	ARandomCarSpawnController()
		: Super()
	{
	}

public:
	/**
	 * Called every frame to update the actor.
	 * Handles random car spawning logic.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to random car spawning.
	 */
	virtual void BeginPlay() override;

	/**
	 * Retrieves a random car source from the list of registered sources.
	 *
	 * @return A pointer to a randomly selected ACarSource.
	 */
	virtual class ACarSource* _GetRandomSource();

	/**
	 * Spawns a car at a randomly selected source.
	 */
	void _SpawnCar();
};

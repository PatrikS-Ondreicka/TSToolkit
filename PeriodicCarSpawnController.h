
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarSpawnController.h"
#include "PeriodicCarSpawnController.generated.h"

/**
 * APeriodicCarSpawnController is a specialized car spawn controller that spawns cars periodically
 * at all registered car sources. It extends the functionality of ACarSpawnController.
 */
UCLASS()
class TSTOOLKIT_API APeriodicCarSpawnController : public ACarSpawnController
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for APeriodicCarSpawnController.
	 * Initializes default values for this controller's properties.
	 */
	APeriodicCarSpawnController();

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to periodic car spawning.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Called every frame to update the actor.
	 * Handles periodic car spawning logic.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

private:
	/**
	 * Spawns cars at all registered car sources.
	 * This method is called periodically to handle car spawning.
	 */
	void _SpawnAtAllSources();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car.h"
#include "CarSpawnController.generated.h"

class ACarSource;

/**
 * ACarSpawnController is responsible for managing car spawning in the simulation.
 * It controls multiple car sources, handles spawn rates, and manages car blueprints.
 */
UCLASS()
class TSTOOLKIT_API ACarSpawnController : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ACarSpawnController.
	 * Sets default values for this actor's properties.
	 */
	ACarSpawnController();

	/** Array of car sources managed by this controller. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	TArray<ACarSource*> Sources;

	/** Whether to automatically register all car sources at BeginPlay. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool bRegisterAllAtBeginPlay = true;

	/** The rate at which cars are spawned, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float SpawnRate = 10.0f;

	/** Pool of car blueprints available for spawning. */
	UPROPERTY(VisibleAnywhere, Category = "Controller Details")
	TArray<TSubclassOf<ACar>> CarBpPool;

	/** Static array of paths to car blueprints. */
	static TArray<FString> CarBpPaths;

protected:
	/** Indicates whether it is currently night time in the simulation. */
	bool _IsNight = false;

	/** Indicates whether the spawn timer has run out. */
	bool _TimerRunOut = false;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Sets up the round by initializing necessary components or states.
	 */
	virtual void _RoundSetUp();

	/**
	 * Handles actions to be performed when the spawn timer runs out.
	 */
	virtual void _TimerAction();

	/**
	 * Checks whether all sources are ready to spawn cars.
	 *
	 * @return True if all sources can spawn cars, false otherwise.
	 */
	bool _CanSourcesSpawn();

	/**
	 * Retrieves a random car class from the blueprint pool.
	 *
	 * @return A random TSubclassOf<ACar> from the CarBpPool.
	 */
	TSubclassOf<ACar> _GetRandomCarClass();

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Checks whether it is currently night time in the simulation.
	 *
	 * @return True if it is night, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsNight() const 
	{ 
		return _IsNight; 
	}

	/**
	 * Sets whether it is night time in the simulation.
	 *
	 * @param State True to set night time, false to set day time.
	 */
	UFUNCTION(BlueprintCallable)
	void SetNight(bool State);

private:
	/**
	 * Registers all car sources in the simulation.
	 */
	void _RegisterAllSources();
};

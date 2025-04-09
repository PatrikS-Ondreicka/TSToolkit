// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car.h"
#include "CarPath.h"
#include "CarSource.generated.h"

/**
 * ACarSource is a class representing a source that spawns cars into the simulation.
 * It manages car spawning, path selection, and car behavior customization.
 */
UCLASS(Blueprintable)
class TSTOOLKIT_API ACarSource : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ACarSource.
	 * Sets default values for this actor's properties.
	 */
	ACarSource();

	// Components
	/** Box component representing the root of the car source. */
	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UBoxComponent* SourceBoxRoot;

	/** Static mesh component representing the visual appearance of the car source. */
	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UStaticMeshComponent* SourceMesh;

	/** Box component used to check for spawn conditions. */
	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UBoxComponent* SpawnCheckBox;

	/** Array of paths that cars spawned from this source can follow. */
	UPROPERTY(EditAnywhere, Category = "Source Components")
	TArray<ACarPath*> Paths;

	// Details
	/** The default car class to spawn. */
	UPROPERTY(EditAnywhere, Category = "Source Details")
	TSubclassOf<ACar> DefaultCarClass;

	/** The maximum number of cars that can be spawned from this source. */
	UPROPERTY(EditAnywhere, Category = "Source Details")
	int MaxCarsCount = 5;

	/** Reference to the car spawn controller managing this source. */
	UPROPERTY(EditAnywhere, Category = "Source Details")
	class ACarSpawnController* Controller = nullptr;

	/** Indicates whether it is night time, affecting car behavior. */
	UPROPERTY(EditAnywhere, Category = "Source Details")
	bool IsNight = false;

	// Car behavior details
	/** The static speed of cars spawned from this source. */
	UPROPERTY(EditAnywhere, Category = "Spawned Car Details")
	float CarStaticSpeed = 10;

	/** The movement priority of cars spawned from this source. */
	UPROPERTY(EditAnywhere, Category = "Spawned Car Details")
	int CarMovementPriority = 100;

private:
	/** Indicates whether the source can currently spawn cars. */
	bool _CanSpawn = true;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called when the actor is constructed or properties are changed in the editor.
	 * Allows for dynamic updates to the actor's properties.
	 *
	 * @param Transform The transform of the actor.
	 */
	void OnConstruction(const FTransform& Transform) override;

	/**
	 * Spawns a car using the default car class.
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnDefaultCar();

	/**
	 * Spawns a car using the specified car class.
	 *
	 * @param CarClass The class of the car to spawn.
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnCar(TSubclassOf<ACar> CarClass);

	// Getter and Setter for _CanSpawn
	/**
	 * Sets whether the source can spawn cars.
	 *
	 * @param CanSpawnValue True if the source can spawn cars, false otherwise.
	 */
	FORCEINLINE void SetCanSpawn(bool CanSpawnValue)
	{
		_CanSpawn = CanSpawnValue;
	}

	/**
	 * Gets whether the source can spawn cars.
	 *
	 * @return True if the source can spawn cars, false otherwise.
	 */
	FORCEINLINE bool GetCanSpawn() const
	{
		return _CanSpawn;
	}

private:
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
	UFUNCTION()
	virtual void _OnSpawnCheckBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/**
	 * Handles the event when another actor ends overlapping with the spawn check box.
	 *
	 * @param OverlappedComponent The component that was overlapped.
	 * @param OtherActor The other actor involved in the overlap.
	 * @param OtherComp The other component involved in the overlap.
	 * @param OtherBodyIndex The body index of the other component.
	 */
	UFUNCTION()
	virtual void _OnSpawnCheckEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	/**
	 * Selects a path for a spawned car to follow.
	 *
	 * @return A pointer to the selected car path.
	 */
	ACarPath* _SelectPath();

	/**
	 * Initializes the paths for the car source.
	 */
	void _InitPath();
};
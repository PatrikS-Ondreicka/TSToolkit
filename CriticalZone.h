// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarPath.h"
#include "CriticalZone.generated.h"

/**
 * ACriticalZone represents a critical zone in the simulation where cars may need to wait
 * or reserve access to ensure safe and orderly movement.
 */
UCLASS()
class TSTOOLKIT_API ACriticalZone : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ACriticalZone.
	 * Sets default values for this actor's properties.
	 */
	ACriticalZone();

	/** Box component defining the bounds of the critical zone. */
	UPROPERTY(EditAnywhere, Category = "Critical Zone Components")
	class UBoxComponent* BoxComponent;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

private:
	/** The car path currently reserving this critical zone. */
	class ACarPath* _CurrentPath;

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Checks if the critical zone is currently reserved.
	 *
	 * @return True if the critical zone is reserved, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	FORCEINLINE bool IsReserved() const
	{
		return _CurrentPath != nullptr;
	}

	/**
	 * Sets the reservation for the critical zone to a specific car path.
	 *
	 * @param Path The car path reserving the critical zone.
	 */
	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	FORCEINLINE void SetReserved(ACarPath* Path)
	{
		_CurrentPath = Path;
	}

	/**
	 * Checks if the critical zone is reserved for a specific car path.
	 *
	 * @param Path The car path to check.
	 * @return True if the critical zone is reserved for the specified path, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	bool IsReservedForPath(ACarPath* Path);

	/**
	 * Attempts to end the reservation of the critical zone.
	 * If the critical zone is reserved, it will release the reservation.
	 */
	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	void TryEndReservation();
};

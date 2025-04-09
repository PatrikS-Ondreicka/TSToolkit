
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLights.generated.h"

/**
 * Enum representing the states of traffic lights.
 * - Green: Cars can move.
 * - Orange: Cars should prepare to stop.
 * - Red: Cars must stop.
 */
UENUM()
enum class ETrafficLightsStates
{
	Green,
	Orange,
	Red
};

/**
 * ATrafficLights represents a traffic light system in the simulation.
 * It includes components for the traffic light's visual representation and logic for controlling its state.
 */
UCLASS()
class TSTOOLKIT_API ATrafficLights : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ATrafficLights.
	 * Sets default values for this actor's properties.
	 */
	ATrafficLights();

	/** Box component representing the area affected by the traffic lights. */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UBoxComponent* TrafficLightsEffectBox;

	/** Static mesh component representing the traffic light's physical structure. */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UStaticMeshComponent* TrafficLightsMeshComponent;

	/** Spotlight component representing the green light. */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class USpotLightComponent* GreenLightComponent;

	/** Spotlight component representing the orange light. */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class USpotLightComponent* OrangeLightComponent;

	/** Spotlight component representing the red light. */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class USpotLightComponent* RedLightComponent;

	/** Current state of the traffic lights (Green, Orange, or Red). */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Details")
	ETrafficLightsStates CurrentState = ETrafficLightsStates::Green;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to the traffic lights.
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
	 * Sets the state of the traffic lights.
	 *
	 * @param NewState The new state to set (Green, Orange, or Red).
	 */
	UFUNCTION(BlueprintCallable)
	void SetTrafficLightsState(ETrafficLightsStates NewState);

	/**
	 * Checks if the traffic light is in the Green state.
	 *
	 * @return True if the traffic light is green, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGreen()
	{
		return CurrentState == ETrafficLightsStates::Green;
	}

	/**
	 * Checks if the traffic light is in the Orange state.
	 *
	 * @return True if the traffic light is orange, false otherwise.
	 */
	FORCEINLINE bool IsOrange()
	{
		return CurrentState == ETrafficLightsStates::Orange;
	}

	/**
	 * Checks if the traffic light is in the Red state.
	 *
	 * @return True if the traffic light is red, false otherwise.
	 */
	FORCEINLINE bool IsRed()
	{
		return CurrentState == ETrafficLightsStates::Red;
	}

protected:
	/**
	 * Sets whether cars are allowed to move based on the traffic light's state.
	 *
	 * @param CarsMoveValue True if cars are allowed to move, false otherwise.
	 */
	void _SetCarsMove(bool CarsMoveValue);
};

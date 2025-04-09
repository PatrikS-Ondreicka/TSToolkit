// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLights.h"
#include "TrafficLightsGroup.generated.h"

class ATrafficLights;

/**
 * ATrafficLightsGroup represents a group of traffic lights that can be controlled together.
 * It allows setting the state of all traffic lights in the group and manages state changes over time.
 */
UCLASS()
class TSTOOLKIT_API ATrafficLightsGroup : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ATrafficLightsGroup.
	 * Sets default values for this actor's properties.
	 */
	ATrafficLightsGroup();

	/** List of traffic lights managed by this group. */
	UPROPERTY(EditAnywhere, Category = "Group Details")
	TArray<ATrafficLights*> TrafficLightsList;

	/** Time interval for changing the state of the traffic lights, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Group Details")
	float StateChangeTime = 5.0f;

	/** Default state of the traffic lights in the group (Green, Orange, or Red). */
	UPROPERTY(EditAnywhere, Category = "Group Details")
	ETrafficLightsStates DefaultState = ETrafficLightsStates::Red;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to the traffic lights group.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes the list of traffic lights in the group.
	 * This method is called during BeginPlay to populate the TrafficLightsList.
	 */
	virtual void _InitTrafficLightsList();

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Sets the state of all traffic lights in the group.
	 *
	 * @param NewState The new state to set for the traffic lights (Green, Orange, or Red).
	 */
	UFUNCTION()
	void SetGroupState(ETrafficLightsStates NewState);
};

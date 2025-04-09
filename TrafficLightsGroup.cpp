// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficLightsGroup.h"

/**
 * Constructor for ATrafficLightsGroup.
 * Initializes default values for the traffic lights group.
 */
ATrafficLightsGroup::ATrafficLightsGroup()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes the traffic lights list and sets their default state.
 */
void ATrafficLightsGroup::BeginPlay()
{
	Super::BeginPlay();
	_InitTrafficLightsList();
}

/**
 * Initializes the list of traffic lights in the group.
 * Sets the default state for all traffic lights in the list.
 */
void ATrafficLightsGroup::_InitTrafficLightsList()
{
	if (TrafficLightsList.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("_InitTrafficLightsList: TrafficLightsList is empty."));
		return;
	}

	for (ATrafficLights* trafficLights : TrafficLightsList)
	{
		if (!trafficLights)
		{
			UE_LOG(LogTemp, Warning, TEXT("_InitTrafficLightsList: Null traffic light found in TrafficLightsList."));
			continue;
		}

		trafficLights->SetTrafficLightsState(DefaultState);
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ATrafficLightsGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Sets the state of all traffic lights in the group.
 *
 * @param NewState The new state to set for the traffic lights (Green, Orange, or Red).
 */
void ATrafficLightsGroup::SetGroupState(ETrafficLightsStates NewState)
{
	if (TrafficLightsList.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetGroupState: TrafficLightsList is empty."));
		return;
	}

	for (ATrafficLights* trafficLights : TrafficLightsList)
	{
		if (!trafficLights)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetGroupState: Null traffic light found in TrafficLightsList."));
			continue;
		}

		trafficLights->SetTrafficLightsState(NewState);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficLightsGroup.h"

// Sets default values
ATrafficLightsGroup::ATrafficLightsGroup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATrafficLightsGroup::BeginPlay()
{
	Super::BeginPlay();
	_InitTrafficLightsList();
}

void ATrafficLightsGroup::_InitTrafficLightsList()
{
	if (TrafficLightsList.Num() <= 0)
	{
		return;
	}

	for (auto trafficLights : TrafficLightsList)
	{
		trafficLights->SetTrafficLightsState(DefaultState);
	}
}

// Called every frame
void ATrafficLightsGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrafficLightsGroup::SetGroupState(ETrafficLightsStates NewState)
{
	if (TrafficLightsList.Num() <= 0)
	{
		return;
	}

	for (auto trafficLights : TrafficLightsList)
	{
		trafficLights->SetTrafficLightsState(NewState);
	}
}
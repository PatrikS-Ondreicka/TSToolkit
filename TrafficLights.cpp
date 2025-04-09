// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficLights.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Car.h"

/**
 * Constructor for ATrafficLights.
 * Initializes the traffic light's components and sets default properties.
 */
ATrafficLights::ATrafficLights()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the traffic light's mesh component
	TrafficLightsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightsMeshComponent"));
	if (!TrafficLightsMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create TrafficLightsMeshComponent in ATrafficLights constructor."));
	}
	SetRootComponent(TrafficLightsMeshComponent);

	// Initialize the effect box component
	TrafficLightsEffectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TrafficLightsEffectBox"));
	if (!TrafficLightsEffectBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create TrafficLightsEffectBox in ATrafficLights constructor."));
	}
	TrafficLightsEffectBox->SetupAttachment(RootComponent);

	// Initialize the green light component
	GreenLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("GreenLightComponent"));
	if (!GreenLightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GreenLightComponent in ATrafficLights constructor."));
	}
	GreenLightComponent->SetupAttachment(RootComponent);

	// Initialize the orange light component
	OrangeLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("OrangeLightComponent"));
	if (!OrangeLightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create OrangeLightComponent in ATrafficLights constructor."));
	}
	OrangeLightComponent->SetupAttachment(RootComponent);

	// Initialize the red light component
	RedLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("RedLightComponent"));
	if (!RedLightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create RedLightComponent in ATrafficLights constructor."));
	}
	RedLightComponent->SetupAttachment(RootComponent);
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes the traffic light's state and sets up overlap delegates.
 */
void ATrafficLights::BeginPlay()
{
	Super::BeginPlay();

	// Ensure components are valid
	if (!TrafficLightsEffectBox)
	{
		UE_LOG(LogTemp, Error, TEXT("TrafficLightsEffectBox is null in BeginPlay."));
		return;
	}

	// Set the initial state of the traffic lights
	SetTrafficLightsState(CurrentState);

	// Uncomment if overlap functionality is needed
	// TrafficLightsEffectBox->OnComponentBeginOverlap.AddDynamic(this, &ATrafficLights::_OnBeginOverlap);
	// TrafficLightsEffectBox->OnComponentEndOverlap.AddDynamic(this, &ATrafficLights::_OnEndOverlap);
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ATrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Sets the state of the traffic lights and updates the visibility of the light components.
 *
 * @param NewState The new state to set (Green, Orange, or Red).
 */
void ATrafficLights::SetTrafficLightsState(ETrafficLightsStates NewState)
{
	CurrentState = NewState;

	if (!GreenLightComponent || !OrangeLightComponent || !RedLightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("One or more light components are null in SetTrafficLightsState."));
		return;
	}

	// Update the visibility of the light components based on the new state
	GreenLightComponent->SetVisibility(ETrafficLightsStates::Green == NewState);
	OrangeLightComponent->SetVisibility(ETrafficLightsStates::Orange == NewState);
	RedLightComponent->SetVisibility(ETrafficLightsStates::Red == NewState);

	// Allow cars to move if the light is green
	if (NewState == ETrafficLightsStates::Green)
	{
		_SetCarsMove(true);
	}
}

/**
 * Sets whether cars are allowed to move based on the traffic light's state.
 *
 * @param CarsMoveValue True if cars are allowed to move, false otherwise.
 */
void ATrafficLights::_SetCarsMove(bool CarsMoveValue)
{
	if (!TrafficLightsEffectBox)
	{
		UE_LOG(LogTemp, Error, TEXT("TrafficLightsEffectBox is null in _SetCarsMove."));
		return;
	}

	// Get all actors overlapping the effect box
	TArray<AActor*> overlappingActors;
	TrafficLightsEffectBox->GetOverlappingActors(overlappingActors);

	// Update the movement state of overlapping cars
	for (AActor* actor : overlappingActors)
	{
		if (!actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Null actor found in _SetCarsMove."));
			continue;
		}

		ACar* car = Cast<ACar>(actor);
		if (!car)
		{
			continue;
		}

		car->SetCanMove(CarsMoveValue);
	}
}

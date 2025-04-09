// Fill out your copyright notice in the Description page of Project Settings.

#include "Lamp.h"
#include "WeatherController.h"
#include "Components/SpotLightComponent.h"

/**
 * Constructor for ALamp.
 * Initializes the spotlight component and sets default properties for the lamp.
 */
ALamp::ALamp()
	: Super()
{
	// Create and attach the spotlight component
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Lamp SpotLight"));
	if (!SpotLight)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SpotLight in ALamp constructor."));
	}
	SpotLight->SetupAttachment(RootComponent);

	// Set default intensity units for the spotlight
	if (SpotLight)
	{
		SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	}
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes the lamp's state based on its properties.
 */
void ALamp::BeginPlay()
{
	Super::BeginPlay();

	if (!SpotLight)
	{
		UE_LOG(LogTemp, Error, TEXT("SpotLight is null in BeginPlay."));
		return;
	}

	// Set the initial state of the lamp based on bSetOnBeginPlay and bIsOn
	if (!bSetOnBeginPlay)
	{
		return;
	}

	if (bIsOn)
	{
		TurnOn();
	}
	else
	{
		TurnOff();
	}
}

/**
 * Turns the lamp on by enabling the spotlight component and setting its intensity.
 */
void ALamp::TurnOn()
{
	if (!SpotLight)
	{
		UE_LOG(LogTemp, Error, TEXT("SpotLight is null in TurnOn."));
		return;
	}

	SpotLight->SetIntensity(36000);
	bIsOn = true;
}

/**
 * Turns the lamp off by disabling the spotlight component and setting its intensity to zero.
 */
void ALamp::TurnOff()
{
	if (!SpotLight)
	{
		UE_LOG(LogTemp, Error, TEXT("SpotLight is null in TurnOff."));
		return;
	}

	SpotLight->SetIntensity(0);
	bIsOn = false;
}

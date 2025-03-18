// Fill out your copyright notice in the Description page of Project Settings.

#include "Lamp.h"
#include "WeatherController.h"
#include "Components/SpotLightComponent.h"

ALamp::ALamp()
	: Super()
{
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Lamp SpotLight"));
	SpotLight->SetupAttachment(RootComponent);

	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
}

void ALamp::BeginPlay()
{
	Super::BeginPlay();

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

void ALamp::TurnOn()
{
	SpotLight->SetIntensity(36000);
	bIsOn = true;
}

void ALamp::TurnOff()
{
	SpotLight->SetIntensity(0);
	bIsOn = false;
}
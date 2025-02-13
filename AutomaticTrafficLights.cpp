// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticTrafficLights.h"

AAutomaticTrafficLights::AAutomaticTrafficLights()
	: Super::ATrafficLights()
{
	_countdown = RedToGreenLightCountDownTime;
}

void AAutomaticTrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Simulation of light changes
	_countdown -= DeltaTime;
	if (_countdown <= 0)
	{
		_changeStateAfterCountdown();
	}
}

void AAutomaticTrafficLights::_changeStateAfterCountdown()
{
	_countdown = RedToGreenLightCountDownTime;
	SetTrafficLightsState((CurrentState == ETrafficLightsStates::Red)
		? ETrafficLightsStates::Green
		: ETrafficLightsStates::Red);
}



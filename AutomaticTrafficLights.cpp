// Fill out your copyright notice in the Description page of Project Settings.

#include "AutomaticTrafficLights.h"

AAutomaticTrafficLights::AAutomaticTrafficLights()
	: Super::ATrafficLights()
{
	_Countdown = RedToGreenLightCountDownTime;
}

void AAutomaticTrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Simulation of light changes
	_Countdown -= DeltaTime;
	if (_Countdown <= 0)
	{
		_ChangeStateAfterCountdown();
	}
}

void AAutomaticTrafficLights::_ChangeStateAfterCountdown()
{
	_Countdown = RedToGreenLightCountDownTime;
	SetTrafficLightsState((CurrentState == ETrafficLightsStates::Red)
		? ETrafficLightsStates::Green
		: ETrafficLightsStates::Red);
}
#include "AutomaticTrafficLights.h"
#include "GameFramework/Actor.h" // For logging
#include "Engine/Engine.h" // For GEngine logging

AAutomaticTrafficLights::AAutomaticTrafficLights()
	: Super::ATrafficLights()
{
	// Ensure the countdown timer is valid
	if (RedToGreenLightCountDownTime <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("RedToGreenLightCountDownTime is invalid. Setting to default value of 5.0f."));
		RedToGreenLightCountDownTime = 5.0f;
	}

	_Countdown = RedToGreenLightCountDownTime;
}

void AAutomaticTrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ensure DeltaTime is valid
	if (DeltaTime <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeltaTime is invalid. Skipping Tick update."));
		return;
	}

	// Decrease the countdown timer
	_Countdown -= DeltaTime;
	if (_Countdown <= 0)
	{
		_ChangeStateAfterCountdown();
	}
}

void AAutomaticTrafficLights::_ChangeStateAfterCountdown()
{
	// Reset the countdown timer
	_Countdown = RedToGreenLightCountDownTime;

	// Validate the current state before toggling
	if (CurrentState != ETrafficLightsStates::Red && CurrentState != ETrafficLightsStates::Green)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state detected. Defaulting to Red."));
		SetTrafficLightsState(ETrafficLightsStates::Red);
		return;
	}

	// Toggle the traffic light state
	SetTrafficLightsState((CurrentState == ETrafficLightsStates::Red)
		? ETrafficLightsStates::Green
		: ETrafficLightsStates::Red);
}
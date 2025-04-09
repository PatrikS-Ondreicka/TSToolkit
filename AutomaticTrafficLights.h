// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficLights.h"
#include "AutomaticTrafficLights.generated.h"

/**
 * AAutomaticTrafficLights is a class that represents an automatic traffic light system.
 * It inherits from the ATrafficLights class and provides functionality to automatically
 * change the traffic light state after a countdown timer.
 */
UCLASS()
class TSTOOLKIT_API AAutomaticTrafficLights : public ATrafficLights
{
	GENERATED_BODY()

public:
	/**
  * Default constructor for AAutomaticTrafficLights.
  * Initializes default values for the traffic light countdown timer.
  */
	AAutomaticTrafficLights();

	/**
  * The time in seconds for the countdown from red to green light.
  * This value can be edited in the editor under the "Traffic Lights Details" category.
  */
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Details")
	float RedToGreenLightCountDownTime = 5.0f;

private:
	/**
  * Internal countdown timer used to track the time remaining before changing the light state.
  */
	float _Countdown;

public:
	/**
  * Called every frame to update the traffic light system.
  * @param DeltaTime - The time elapsed since the last frame.
  */
	virtual void Tick(float DeltaTime) override;

private:
	/**
  * Changes the traffic light state after the countdown timer reaches zero.
  * This method is called internally to handle the state transition logic.
  */
	virtual void _ChangeStateAfterCountdown();
};
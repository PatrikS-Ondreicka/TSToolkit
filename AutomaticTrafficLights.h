// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficLights.h"
#include "AutomaticTrafficLights.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API AAutomaticTrafficLights : public ATrafficLights
{
	GENERATED_BODY()

public:
	AAutomaticTrafficLights();

	UPROPERTY(EditAnywhere, Category = "Traffic Lights Details")
	float RedToGreenLightCountDownTime = 5.0;
	
private:
	float _countdown;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void _changeStateAfterCountdown();
};

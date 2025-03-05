// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecorativeActor.h"
#include "Lamp.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API ALamp : public ADecorativeActor
{
	GENERATED_BODY()
	
public:
	ALamp();

	UPROPERTY(EditAnywhere, Category = "Lamp Components")
	class USpotLightComponent* SpotLight;

	UPROPERTY(EditAnywhere, Category = "Lamp Details")
	bool IsOn = false;

	UPROPERTY(EditAnywhere, Category = "Lamp Details")
	bool SetOnBeginPlay = false;

	UPROPERTY(EditAnywhere, Category = "Lamp Details")
	bool TurnOnWhenNight = true;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TurnOn();

	UFUNCTION(BlueprintCallable)
	void TurnOff();
};

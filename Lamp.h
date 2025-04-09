// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecorativeActor.h"
#include "Lamp.generated.h"

/**
 * ALamp represents a decorative lamp in the simulation.
 * It extends ADecorativeActor and includes functionality for controlling a spotlight component.
 */
UCLASS()
class TSTOOLKIT_API ALamp : public ADecorativeActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ALamp.
	 * Sets default values for this actor's properties.
	 */
	ALamp();

	/** Spotlight component representing the light emitted by the lamp. */
	UPROPERTY(EditAnywhere, Category = "Lamp Components")
	class USpotLightComponent* SpotLight;

	/** Indicates whether the lamp is currently turned on. */
	UPROPERTY(EditAnywhere, Category = "Lamp Details")
	bool bIsOn = false;

	/** Indicates whether the lamp should be turned on at the start of the game. */
	UPROPERTY(EditAnywhere, Category = "Lamp Details")
	bool bSetOnBeginPlay = false;

	/** Indicates whether the lamp should automatically turn on at night. */
	UPROPERTY(EditAnywhere, Category = "Lamp Details")
	bool bTurnOnWhenNight = true;

public:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Initializes the lamp's state based on its properties.
	 */
	virtual void BeginPlay() override;

	/**
	 * Turns the lamp on by enabling the spotlight component.
	 */
	UFUNCTION(BlueprintCallable)
	void TurnOn();

	/**
	 * Turns the lamp off by disabling the spotlight component.
	 */
	UFUNCTION(BlueprintCallable)
	void TurnOff();
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorativeActor.h"
#include "Puddle.generated.h"

/**
 * APuddle represents a decorative puddle in the simulation.
 * It extends ADecorativeActor and inherits its functionality.
 */
UCLASS()
class TSTOOLKIT_API APuddle : public ADecorativeActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for APuddle.
	 * Sets default values for this actor's properties.
	 */
	APuddle();

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to the puddle.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;
};

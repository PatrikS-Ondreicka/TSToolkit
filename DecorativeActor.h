// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorativeActor.generated.h"

/**
 * ADecorativeActor represents a decorative object in the simulation.
 * It includes a root box component and a static mesh component for visualization.
 */
UCLASS()
class TSTOOLKIT_API ADecorativeActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ADecorativeActor.
	 * Sets default values for this actor's properties.
	 */
	ADecorativeActor();

	/** Box component serving as the root of the decorative actor. */
	UPROPERTY(EditAnywhere, Category = "Decorative Actor Components")
	class UBoxComponent* DecorativeActorRootBox;

	/** Static mesh component representing the visual appearance of the decorative actor. */
	UPROPERTY(EditAnywhere, Category = "Decorative Actor Components")
	class UStaticMeshComponent* DecorativeActorMeshComponent;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
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

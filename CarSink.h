
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarSink.generated.h"

/**
 * ACarSink is a class representing a sink area where cars are removed from the simulation.
 * It uses a box component to detect overlaps with cars and handles their removal.
 */
UCLASS()
class TSTOOLKIT_API ACarSink : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ACarSink.
	 * Sets default values for this actor's properties.
	 */
	ACarSink();

	/** Box component used to define the sink area and detect overlaps with cars. */
	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UBoxComponent* SinkBoxRoot;

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

private:
	/**
	 * Handles the event when another actor begins overlapping with the sink box.
	 *
	 * @param OverlappedComponent The component that was overlapped.
	 * @param OtherActor The other actor involved in the overlap.
	 * @param OtherComp The other component involved in the overlap.
	 * @param OtherBodyIndex The body index of the other component.
	 * @param bFromSweep Whether the overlap was caused by a sweep.
	 * @param SweepResult The result of the sweep.
	 */
	UFUNCTION()
	virtual void _onCarSinkBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
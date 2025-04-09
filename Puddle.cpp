// Fill out your copyright notice in the Description page of Project Settings.

#include "Puddle.h"

/**
 * Constructor for APuddle.
 * Sets default values for the puddle actor's properties.
 */
APuddle::APuddle()
{
	// Set this actor to not call Tick() every frame to improve performance.
	PrimaryActorTick.bCanEverTick = false;
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic specific to the puddle.
 */
void APuddle::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void APuddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

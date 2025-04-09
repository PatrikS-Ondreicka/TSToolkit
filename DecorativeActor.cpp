// Fill out your copyright notice in the Description page of Project Settings.

#include "DecorativeActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

/**
 * Constructor for ADecorativeActor.
 * Sets default values for the decorative actor's components and initializes the actor.
 */
ADecorativeActor::ADecorativeActor()
{
	// Set this actor to not call Tick() every frame to improve performance.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize the root box component
	DecorativeActorRootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Decorative Actor Root Box Component"));
	if (!DecorativeActorRootBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create DecorativeActorRootBox in ADecorativeActor constructor."));
	}
	SetRootComponent(DecorativeActorRootBox);

	// Initialize the static mesh component
	DecorativeActorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decorative Actor Static Mesh"));
	if (!DecorativeActorMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create DecorativeActorMeshComponent in ADecorativeActor constructor."));
	}
	DecorativeActorMeshComponent->SetupAttachment(RootComponent);
	DecorativeActorMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic.
 */
void ADecorativeActor::BeginPlay()
{
	Super::BeginPlay();

	if (!DecorativeActorRootBox)
	{
		UE_LOG(LogTemp, Error, TEXT("DecorativeActorRootBox is null in BeginPlay."));
	}

	if (!DecorativeActorMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("DecorativeActorMeshComponent is null in BeginPlay."));
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ADecorativeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

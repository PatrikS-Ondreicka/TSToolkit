// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSink.h"
#include "Car.h"
#include "Components/BoxComponent.h"

/**
 * Constructor for ACarSink.
 * Sets default values for the sink's properties and initializes the sink box component.
 */
ACarSink::ACarSink()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the sink box component
	SinkBoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Root Box Component"));
	if (!SinkBoxRoot)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SinkBoxRoot in ACarSink constructor."));
	}
	SetRootComponent(SinkBoxRoot);
}

/**
 * Called when the game starts or when the actor is spawned.
 * Sets up overlap event bindings for the sink box component.
 */
void ACarSink::BeginPlay()
{
	Super::BeginPlay();

	if (!SinkBoxRoot)
	{
		UE_LOG(LogTemp, Error, TEXT("SinkBoxRoot is null in BeginPlay."));
		return;
	}

	// Bind the overlap event
	SinkBoxRoot->OnComponentBeginOverlap.AddDynamic(this, &ACarSink::_onCarSinkBeginOverlap);
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACarSink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Handles the event when another actor begins overlapping with the sink box.
 * If the overlapping actor is a car, it marks the car as having reached its destination.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The other actor involved in the overlap.
 * @param OtherComp The other component involved in the overlap.
 * @param OtherBodyIndex The body index of the other component.
 * @param bFromSweep Whether the overlap was caused by a sweep.
 * @param SweepResult The result of the sweep.
 */
void ACarSink::_onCarSinkBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("_onCarSinkBeginOverlap called with a null OtherActor."));
		return;
	}

	ACar* other = Cast<ACar>(OtherActor);
	if (!other)
	{
		UE_LOG(LogTemp, Warning, TEXT("_onCarSinkBeginOverlap: Overlapping actor is not a car."));
		return;
	}

	// Mark the car as having reached its destination
	other->SetReachedDestination(true);
}

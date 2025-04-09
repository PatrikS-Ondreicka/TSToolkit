// Fill out your copyright notice in the Description page of Project Settings.

#include "CriticalZone.h"
#include "Components/BoxComponent.h"
#include "CarPath.h"
#include "Car.h"

/**
 * Constructor for ACriticalZone.
 * Sets default values for the critical zone's components and initializes the actor.
 */
ACriticalZone::ACriticalZone()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the box component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Critical Zone Box Component"));
	if (!BoxComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create BoxComponent in ACriticalZone constructor."));
	}
	SetRootComponent(BoxComponent);
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic.
 */
void ACriticalZone::BeginPlay()
{
	Super::BeginPlay();

	if (!BoxComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("BoxComponent is null in BeginPlay."));
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACriticalZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Debug message to display whether the critical zone is reserved
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("IsReserved: %d"), IsReserved()));
	}
}

/**
 * Checks if the critical zone is reserved for a specific car path.
 *
 * @param Path The car path to check.
 * @return True if the critical zone is reserved for the specified path, false otherwise.
 */
bool ACriticalZone::IsReservedForPath(ACarPath* Path)
{
	if (!Path)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsReservedForPath called with a null Path."));
		return false;
	}

	if (!_CurrentPath)
	{
		return false;
	}

	if (_CurrentPath == Path)
	{
		return true;
	}

	for (ACarPath* relatedPath : _CurrentPath->RelatedPaths)
	{
		if (relatedPath == Path)
		{
			return true;
		}
	}

	return false;
}

/**
 * Attempts to end the reservation of the critical zone.
 * If no cars are overlapping the critical zone, the reservation is released.
 */
void ACriticalZone::TryEndReservation()
{
	if (!_CurrentPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEndReservation called but _CurrentPath is null."));
		return;
	}

	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors, ACar::StaticClass());

	if (overlappingActors.Num() <= 0)
	{
		_CurrentPath = nullptr;
		return;
	}

	for (AActor* actor : overlappingActors)
	{
		if (!actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("TryEndReservation encountered a null actor in overlappingActors."));
			continue;
		}

		ACar* car = Cast<ACar>(actor);
		if (car && car->GetPath() == _CurrentPath)
		{
			return;
		}
	}

	_CurrentPath = nullptr;
}

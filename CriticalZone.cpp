// Fill out your copyright notice in the Description page of Project Settings.


#include "CriticalZone.h"
#include "Components/BoxComponent.h"
#include "CarPath.h"
#include "Car.h"

// Sets default values
ACriticalZone::ACriticalZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Critical Zone Box Component"));
	SetRootComponent(BoxComponent);
}

// Called when the game starts or when spawned
void ACriticalZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACriticalZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), IsReserved()));
}

bool ACriticalZone::IsReservedForPath(ACarPath* Path)
{
	if (Path == nullptr)
	{
		return false;
	}
	if (_CurrentPath == nullptr)
	{
		return false;
	}
	if (_CurrentPath == Path)
	{
		return true;
	}

	for (ACarPath* path : _CurrentPath->RelatedPaths)
	{
		if (path == Path)
		{
			return true;
		}
	}
	return false;
}

void ACriticalZone::TryEndReservation()
{
	TArray<AActor*> overlappped;
	GetOverlappingActors(overlappped, ACar::StaticClass());
	if (overlappped.Num() <= 0)
	{
		_CurrentPath = nullptr;
		return;
	}
	
	for (AActor* actor : overlappped)
	{
		ACar* car = Cast<ACar>(actor);
		if (car != nullptr)
		{
			if (car->GetPath() == _CurrentPath)
			{
				return;
			}
		}
	}

	_CurrentPath = nullptr;
}


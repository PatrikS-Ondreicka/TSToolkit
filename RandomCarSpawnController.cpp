// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSource.h"
#include "RandomCarSpawnController.h"

#define MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(str));
#define ERROR_MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(str));

void ARandomCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!_Timer->CountdownState())
	{
		_Timer->DecrementCountdown(DeltaTime);
	}
	else
	{
		if (_CanSourcesSpawn())
		{
			_SpawnCar();
			_RoundSetUp();
		}
	}
}

void ARandomCarSpawnController::BeginPlay()
{
	Super::BeginPlay();
}

ACarSource* ARandomCarSpawnController::_GetRandomSource()
{
	if (Sources.Num() <= 0)
	{
		return nullptr;
	}

	int index = FMath::RandRange(0, Sources.Num() - 1);
	ACarSource* randomSource = Sources[index];
	return randomSource;
}

void ARandomCarSpawnController::_SpawnCar()
{
	ACarSource* source = _GetRandomSource();
	if (source)
	{
		if (source->GetCanSpawn())
		{
			source->SpawnCar(_GetRandomCarClass());
		}
		else
		{
			ERROR_MSG("Car spawn was unable to spawn a car");
		}
	}
}
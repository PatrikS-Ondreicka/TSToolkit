// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSource.h"
#include "RandomCarSpawnController.h"

#define MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(str));
#define ERROR_MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(str));

void ARandomCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_currentIntervalCountdown -= DeltaTime;
	_currentCounddownBetweenSpawns -= DeltaTime;

	if (_currentCounddownBetweenSpawns <= 0)
	{
		SpawnCar();
	}

	if (_currentIntervalCarsSpawned >= CarsSpawnedPerInterval)
	{
		_currentIntervalCountdown = TimeInterval;
		_currentIntervalCarsSpawned = 0;
		_currentCounddownBetweenSpawns = _currentTimeBetweenSpawns;
	}
}

void ARandomCarSpawnController::BeginPlay()
{
	Super::BeginPlay();

	_currentIntervalCarsSpawned = 0;
	_currentIntervalCountdown = TimeInterval;
	_currentTimeBetweenSpawns = GetTimeBetweenSpawns();
	_currentCounddownBetweenSpawns = _currentTimeBetweenSpawns;
}

ACarSource* ARandomCarSpawnController::GetRandomSource()
{
	if (Sources.Num() <= 0)
	{
		return nullptr;
	}
	
	int index = FMath::RandRange(0, Sources.Num() - 1);
	ACarSource* randomSource =  Sources[index];
	return randomSource;
}

float ARandomCarSpawnController::GetTimeBetweenSpawns()
{
	if (TimeInterval <= 0 || CarsSpawnedPerInterval <= 0)
	{
		return 0.0f;
	}

	return TimeInterval / CarsSpawnedPerInterval;
}

void ARandomCarSpawnController::SpawnCar()
{
	ACarSource* source = GetRandomSource();
	if (source)
	{
		if (source->GetCanSpawn())
		{
			source->SpawnCar();
		}
		else
		{
			ERROR_MSG("Car spawn was unable to spawn a car");
		}
		_currentCounddownBetweenSpawns = _currentTimeBetweenSpawns;
		_currentIntervalCarsSpawned++;
	}
}

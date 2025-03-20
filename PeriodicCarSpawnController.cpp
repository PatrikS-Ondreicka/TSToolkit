// Fill out your copyright notice in the Description page of Project Settings.

#include "PeriodicCarSpawnController.h"
#include "CarSource.h"

APeriodicCarSpawnController::APeriodicCarSpawnController()
	: Super()
{
}

void APeriodicCarSpawnController::BeginPlay()
{
	Super::BeginPlay();
}

void APeriodicCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!_TimerRunOut)
	{
		return;
	}

	if (_CanSourcesSpawn())
	{
		_SpawnAtAllSources();
		_RoundSetUp();
	}
}

void APeriodicCarSpawnController::_SpawnAtAllSources()
{
	for (auto CarSource : Sources)
	{
		CarSource->SpawnCar(_GetRandomCarClass());
	}
}
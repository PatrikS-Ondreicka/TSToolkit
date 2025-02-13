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
	if (Sources.Num() > 0)
	{
		_roundSetUp();
	}
}

void APeriodicCarSpawnController::_roundSetUp()
{
	_spawnCountDown = SpawnRate;
}


void APeriodicCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_spawnCountDown > 0) 
	{
		_spawnCountDown -= DeltaTime;
		return;
	}

	if (_canSourcesSpawn())
	{
		_spawnAtAllSources();
		_roundSetUp();
	}
}

void APeriodicCarSpawnController::_spawnAtAllSources()
{
	for (auto carSource : Sources)
	{
		carSource->SpawnCar();
	}
}



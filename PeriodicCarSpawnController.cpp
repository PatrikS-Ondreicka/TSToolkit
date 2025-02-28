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

void APeriodicCarSpawnController::_roundSetUp()
{
	Super::_roundSetUp();
}


void APeriodicCarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_spawnCountdown > 0) 
	{
		_spawnCountdown -= DeltaTime;
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



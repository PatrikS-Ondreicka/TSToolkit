// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSpawnController.h"
#include "CarSource.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACarSpawnController::ACarSpawnController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACarSpawnController::BeginPlay()
{
	Super::BeginPlay();

	if (RegisterAllAtBeginPlay)
	{
		_registerAllSources();
	}

	_roundSetUp();
}

// Called every frame
void ACarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_carsToSpawnLeft <= 0)
	{
		_disableAllSources();
	}

	if (_spawnCountDown <= 0)
	{
		_roundSetUp();
	}
	
	_spawnCountDown -= DeltaTime;
}

void ACarSpawnController::SpawnedCarCall()
{
	_carsToSpawnLeft--;
}

void ACarSpawnController::_registerAllSources()
{
	UWorld* world = GetWorld();
	Sources.Empty();

	TArray<AActor*> found;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACarSource::StaticClass(), found);
	for (AActor* actor : found)
	{
		ACarSource* source = Cast<ACarSource>(actor);
		if (source)
		{
			Sources.Add(source);
		}
	}
}

void ACarSpawnController::_roundSetUp()
{
	_carsToSpawnLeft = CarsSpawned;
	_spawnCountDown = SpawnRate;
	for (ACarSource* source : Sources)
	{
		if (SpawnAtOnce)
		{
			source->SpawnRate = 1.0f;
		}
		else
		{
			source->SpawnRate = FMath::RandRange(0.0f, SpawnRate);
		}
		source->AutoSpawn = true;
	}
}

void ACarSpawnController::_disableAllSources()
{
	for (ACarSource* source : Sources)
	{
		source->AutoSpawn = false;
	}
}


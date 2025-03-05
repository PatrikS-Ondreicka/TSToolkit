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

	if (Sources.Num() > 0)
	{
		_roundSetUp();
	}

	SetNight(_isNight);
}

// Called every frame
void ACarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACarSpawnController::SetNight(bool state)
{
	_isNight = state;
	for (ACarSource* source : Sources)
	{
		source->IsNight = state;
	}
}

void ACarSpawnController::_registerAllSources()
{
	UWorld* world = GetWorld();
	Sources.Empty();

	TArray<AActor*> found;

	UGameplayStatics::GetAllActorsOfClass(world, ACarSource::StaticClass(), found);
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
	_spawnCountdown = SpawnRate;
}

bool ACarSpawnController::_canSourcesSpawn()
{
	if (Sources.Num() <= 0)
	{
		return false;
	}

	for (auto carSource : Sources)
	{
		if (!carSource->GetCanSpawn())
		{
			return false;
		}
	}

	return true;
}



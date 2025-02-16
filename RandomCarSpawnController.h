// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarSpawnController.h"
#include "RandomCarSpawnController.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API ARandomCarSpawnController : public ACarSpawnController
{
	GENERATED_BODY()

public:
	ARandomCarSpawnController()
		: Super::ACarSpawnController()
	{
	}

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float TimeInterval = 60;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	int CarsSpawnedPerInterval = 4;

private:
	int _currentIntervalCarsSpawned = 0;
	float _currentIntervalCountdown = 0.0f;
	float _currentTimeBetweenSpawns = 0;
	float _currentCounddownBetweenSpawns = 0.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual class ACarSource* GetRandomSource();
	float GetTimeBetweenSpawns();
	void SpawnCar();
};

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
		: Super()
	{
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual class ACarSource* _GetRandomSource();
	void _SpawnCar();
};
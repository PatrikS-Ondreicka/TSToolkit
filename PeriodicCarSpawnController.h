// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarSpawnController.h"
#include "PeriodicCarSpawnController.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API APeriodicCarSpawnController : public ACarSpawnController
{
	GENERATED_BODY()

public:
	APeriodicCarSpawnController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void _roundSetUp();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void _spawnAtAllSources();
};

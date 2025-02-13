// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarSpawnController.generated.h"

class ACarSource;

UCLASS()
class TSTOOLKIT_API ACarSpawnController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarSpawnController();

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	TArray<ACarSource*> Sources;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool RegisterAllAtBeginPlay = true;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	int CarsSpawned = 0;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float SpawnRate = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool SpawnAtOnce = false;

private:
	float _spawnCountDown;
	int _carsToSpawnLeft;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnedCarCall();

private:
	void _registerAllSources();
	void _roundSetUp();
	void _disableAllSources();
};

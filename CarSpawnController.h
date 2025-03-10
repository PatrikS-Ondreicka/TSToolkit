// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeriodicTimer.h"
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
	float SpawnRate = 10.0f;

protected:
	UPeriodicTimer* _timer;
	bool _isNight = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void _roundSetUp();
	bool _canSourcesSpawn();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsNight() const { return _isNight; }

	UFUNCTION(BlueprintCallable)
	void SetNight(bool state);

private:
	void _registerAllSources();
};

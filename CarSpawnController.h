// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeriodicTimer.h"
#include "Car.h"
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
	bool bRegisterAllAtBeginPlay = true;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float SpawnRate = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Controller Details")
	TArray<TSubclassOf<ACar>> CarBpPool;

	// Static attributes
	static TArray<FString> CarBpPaths;

protected:
	UPeriodicTimer* _Timer;
	bool _IsNight = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void _RoundSetUp();
	bool _CanSourcesSpawn();
	TSubclassOf<ACar> _GetRandomCarClass();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsNight() const { return _IsNight; }

	UFUNCTION(BlueprintCallable)
	void SetNight(bool State);

private:
	void _RegisterAllSources();
};
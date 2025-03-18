// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLightsGroup.h"
#include "TrafficLightsGroupController.generated.h"

class ATrafficLightsGroup;

UCLASS()
class TSTOOLKIT_API ATrafficLightsGroupController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrafficLightsGroupController();

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	TArray<ATrafficLightsGroup*> TrafficLightsGroups;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool bRegisterAllAtBeginPlay = true;

private:
	int _CurrentGroupIndex = 0;
	float _CurrentGroupCountDown = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NextGroup();

	// Attribute getters
	FORCEINLINE int GetCurrentGroupIndex()
	{
		return _CurrentGroupIndex;
	}

	FORCEINLINE float GetCurrentGroupCountDown()
	{
		return _CurrentGroupCountDown;
	}

	FORCEINLINE ATrafficLightsGroup* GetCurrentGroup()
	{
		if (TrafficLightsGroups.Num() <= 0)
		{
			return nullptr;
		}
		return TrafficLightsGroups[_CurrentGroupIndex];
	}

private:
	void _RegisterAllGroups();
	void _SetStateForGroup(int Index, ETrafficLightsStates State);
};
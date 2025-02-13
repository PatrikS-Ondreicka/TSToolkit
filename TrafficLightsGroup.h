// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLights.h"
#include "TrafficLightsGroup.generated.h"

class ATrafficLights;

UCLASS()
class TSTOOLKIT_API ATrafficLightsGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficLightsGroup();

	UPROPERTY(EditAnywhere, Category = "Group Details")
	TArray<ATrafficLights*> TrafficLightsList;

	UPROPERTY(EditAnywhere, Category = "Group Details")
	float StateChangeTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Group Details")
	ETrafficLightsStates DefaultState = ETrafficLightsStates::Red;

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void _initTrafficLightsList();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetGroupState(ETrafficLightsStates NewState);
};

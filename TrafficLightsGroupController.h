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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

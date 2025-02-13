// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightsGroupController.h"

// Sets default values
ATrafficLightsGroupController::ATrafficLightsGroupController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrafficLightsGroupController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrafficLightsGroupController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


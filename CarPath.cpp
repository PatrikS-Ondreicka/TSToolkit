// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPath.h"
#include "Components/SplineComponent.h"

// Sets default values
ACarPath::ACarPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Path = CreateDefaultSubobject<USplineComponent>("Car Path");
}

// Called when the game starts or when spawned
void ACarPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


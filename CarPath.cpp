// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPath.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
ACarPath::ACarPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Car Path"));
	SetRootComponent(Path);

	StartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Path start"));
	StartMesh->SetupAttachment(RootComponent);

	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Path end"));
	EndMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACarPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACarPath::OnConstruction(const FTransform& Transform)
{
	FVector startLocation = StartMesh->GetComponentLocation();
	FVector endLocation = EndMesh->GetComponentLocation();
	int32 pointsCount = Path->GetNumberOfSplinePoints();
	Path->SetWorldLocationAtSplinePoint(0, startLocation);
	Path->SetWorldLocationAtSplinePoint(pointsCount - 1, endLocation);
}

// Called every frame
void ACarPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


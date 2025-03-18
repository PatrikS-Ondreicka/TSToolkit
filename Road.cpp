// Fill out your copyright notice in the Description page of Project Settings.

#include "Road.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ARoad::ARoad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RoadSpline = CreateDefaultSubobject<USplineComponent>(TEXT("RoadSpline"));
	SetRootComponent(RoadSpline);

	RoadMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("RoadMesh"));
}

// Called when the game starts or when spawned
void ARoad::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoad::OnConstruction(const FTransform& Transform)
{
	_SetUpMesh();
}

void ARoad::_SetUpMesh()
{
	if (!RoadMesh)
	{
		return;
	}
	for (int nodeNumber = 0; nodeNumber < RoadSpline->GetNumberOfSplinePoints() - 1; ++nodeNumber)
	{
		FVector currentNodeLocation = RoadSpline->GetLocationAtSplinePoint(nodeNumber, ESplineCoordinateSpace::Local);
		FVector currentNodeTangent = RoadSpline->GetTangentAtSplinePoint(nodeNumber, ESplineCoordinateSpace::Local);
		USplineMeshComponent* splineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		splineMeshComp->SetStaticMesh(RoadMesh);
		splineMeshComp->SetMobility(EComponentMobility::Movable);
		splineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		splineMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		splineMeshComp->RegisterComponentWithWorld(GetWorld());
		splineMeshComp->AttachToComponent(RoadSpline, FAttachmentTransformRules::KeepRelativeTransform);

		FVector startPoint = RoadSpline->GetLocationAtSplinePoint(nodeNumber, ESplineCoordinateSpace::Local);
		FVector startTangent = RoadSpline->GetTangentAtSplinePoint(nodeNumber, ESplineCoordinateSpace::Local);
		FVector endPoint = RoadSpline->GetLocationAtSplinePoint(nodeNumber + 1, ESplineCoordinateSpace::Local);
		FVector endTangent = RoadSpline->GetTangentAtSplinePoint(nodeNumber + 1, ESplineCoordinateSpace::Local);
		splineMeshComp->SetStartAndEnd(startPoint, startTangent, endPoint, endTangent);
	}
}
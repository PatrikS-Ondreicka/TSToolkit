// Fill out your copyright notice in the Description page of Project Settings.

#include "Road.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"

/**
 * Constructor for ARoad.
 * Initializes the spline component and sets default properties for the road.
 */
ARoad::ARoad()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach the spline component
	RoadSpline = CreateDefaultSubobject<USplineComponent>(TEXT("RoadSpline"));
	if (!RoadSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create RoadSpline in ARoad constructor."));
	}
	SetRootComponent(RoadSpline);

	// Create the static mesh for the road
	RoadMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("RoadMesh"));
	if (!RoadMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create RoadMesh in ARoad constructor."));
	}
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic specific to the road.
 */
void ARoad::BeginPlay()
{
	Super::BeginPlay();

	if (!RoadSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("RoadSpline is null in BeginPlay."));
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ARoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Called when the actor is constructed or properties are changed in the editor.
 * Sets up the road's mesh based on the spline and other properties.
 *
 * @param Transform The transform of the actor.
 */
void ARoad::OnConstruction(const FTransform& Transform)
{
	if (!RoadSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("RoadSpline is null in OnConstruction."));
		return;
	}

	_SetUpMesh();
}

/**
 * Sets up the road's mesh along the spline.
 * This method is called during construction to align the mesh with the spline.
 */
void ARoad::_SetUpMesh()
{
	if (!RoadMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("_SetUpMesh called but RoadMesh is null."));
		return;
	}

	if (!RoadSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetUpMesh called but RoadSpline is null."));
		return;
	}

	// Iterate through the spline points and create spline mesh components
	for (int32 NodeIndex = 0; NodeIndex < RoadSpline->GetNumberOfSplinePoints() - 1; ++NodeIndex)
	{
		// Get the start and end points and tangents for the current segment
		FVector StartPoint = RoadSpline->GetLocationAtSplinePoint(NodeIndex, ESplineCoordinateSpace::Local);
		FVector StartTangent = RoadSpline->GetTangentAtSplinePoint(NodeIndex, ESplineCoordinateSpace::Local);
		FVector EndPoint = RoadSpline->GetLocationAtSplinePoint(NodeIndex + 1, ESplineCoordinateSpace::Local);
		FVector EndTangent = RoadSpline->GetTangentAtSplinePoint(NodeIndex + 1, ESplineCoordinateSpace::Local);

		// Create a new spline mesh component
		USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (!SplineMeshComp)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create SplineMeshComponent in _SetUpMesh."));
			continue;
		}

		// Set up the spline mesh component
		SplineMeshComp->SetStaticMesh(RoadMesh);
		SplineMeshComp->SetMobility(EComponentMobility::Movable);
		SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SplineMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComp->RegisterComponentWithWorld(GetWorld());
		SplineMeshComp->AttachToComponent(RoadSpline, FAttachmentTransformRules::KeepRelativeTransform);

		// Set the start and end points and tangents for the spline mesh
		SplineMeshComp->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
	}
}

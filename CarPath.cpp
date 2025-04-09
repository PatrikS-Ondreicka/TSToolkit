// Fill out your copyright notice in the Description page of Project Settings.

#include "CarPath.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"

/**
 * Constructor for ACarPath.
 * Sets default values for the car path's components and initializes the actor.
 */
ACarPath::ACarPath()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the spline component for the car path
	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Car Path"));
	if (!Path)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SplineComponent for CarPath."));
	}
	SetRootComponent(Path);

	// Initialize the start mesh component
	StartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Path start"));
	if (!StartMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create StartMesh for CarPath."));
	}
	StartMesh->SetupAttachment(RootComponent);

	// Initialize the end mesh component
	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Path end"));
	if (!EndMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create EndMesh for CarPath."));
	}
	EndMesh->SetupAttachment(RootComponent);
}

/**
 * Called when the game starts or when the actor is spawned.
 * Used for initialization logic.
 */
void ACarPath::BeginPlay()
{
	Super::BeginPlay();

	// Ensure components are valid
	if (!Path)
	{
		UE_LOG(LogTemp, Error, TEXT("Path is null in BeginPlay."));
	}
	if (!StartMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("StartMesh is null in BeginPlay."));
	}
	if (!EndMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("EndMesh is null in BeginPlay."));
	}
}

/**
 * Called when the actor is constructed or properties are changed in the editor.
 * Allows for dynamic updates to the actor's properties.
 *
 * @param Transform The transform of the actor.
 */
void ACarPath::OnConstruction(const FTransform& Transform)
{
	// Ensure components are valid
	if (!Path)
	{
		UE_LOG(LogTemp, Error, TEXT("Path is null in OnConstruction."));
	}
	if (!StartMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("StartMesh is null in OnConstruction."));
	}
	if (!EndMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("EndMesh is null in OnConstruction."));
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACarPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Checks if the given path is related to this path.
 *
 * @param OtherPath The other path to check.
 * @return True if the other path is related, false otherwise.
 */
bool ACarPath::IsPathRelated(ACarPath* OtherPath)
{
	if (!OtherPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsPathRelated called with a null OtherPath."));
		return false;
	}

	return RelatedPaths.Contains(OtherPath);
}

/**
 * Adds a relation between this path and another path.
 *
 * @param OtherPath The other path to relate to this path.
 */
void ACarPath::AddPathRelation(ACarPath* OtherPath)
{
	if (!OtherPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddPathRelation called with a null OtherPath."));
		return;
	}
	if (!IsPathRelated(OtherPath))
	{
		RelatedPaths.Add(OtherPath);
	}
}

/**
 * Removes a relation between this path and another path.
 *
 * @param OtherPath The other path to remove from the relations.
 */
void ACarPath::RemovePathRelation(ACarPath* OtherPath)
{
	if (!OtherPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemovePathRelation called with a null OtherPath."));
		return;
	}

	RelatedPaths.Remove(OtherPath);
}

/**
 * Updates the relations of this path with other paths.
 * Ensures bidirectional relationships are maintained.
 */
void ACarPath::UpdateRelations()
{
	if (RelatedPaths.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateRelations called but RelatedPaths is empty."));
		return;
	}

	for (auto path : RelatedPaths)
	{
		if (!path)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateRelations encountered a null path in RelatedPaths."));
			continue;
		}

		if (path == this)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateRelations detected a self-reference. Removing this path from RelatedPaths."));
			RemovePathRelation(this);
		}

		if (!path->IsPathRelated(this))
		{
			path->AddPathRelation(this);
		}

		if (!IsPathRelated(path))
		{
			RemovePathRelation(path);
			path->UpdateRelations();
		}
	}
}
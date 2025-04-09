// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarPath.generated.h"

/**
 * ACarPath is a class representing a path that cars can follow in the simulation.
 * It includes a spline component for defining the path and related meshes for visualization.
 * The class also supports managing relationships with other paths.
 */
UCLASS()
class TSTOOLKIT_API ACarPath : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ACarPath.
	 * Sets default values for the actor's properties.
	 */
	ACarPath();

	/** Spline component representing the path for cars to follow. */
	UPROPERTY(EditAnywhere, Category = "Path Components")
	class USplineComponent* Path;

	/** Static mesh component representing the start of the path. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Components")
	class UStaticMeshComponent* StartMesh;

	/** Static mesh component representing the end of the path. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Components")
	class UStaticMeshComponent* EndMesh;

	/** Probability value associated with this path, used for decision-making. */
	UPROPERTY(EditAnywhere, Category = "Path Details")
	float Probability = 0.0f;

	/** Array of paths related to this path. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Details")
	TArray<ACarPath*> RelatedPaths;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Called when the actor is constructed or properties are changed in the editor.
	 * Allows for dynamic updates to the actor's properties.
	 *
	 * @param Transform The transform of the actor.
	 */
	void OnConstruction(const FTransform& Transform) override;

	// Uncomment and customize this function to handle property changes in the editor.
	// void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Checks if the given path is related to this path.
	 *
	 * @param OtherPath The other path to check.
	 * @return True if the other path is related, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	bool IsPathRelated(ACarPath* OtherPath);

	/**
	 * Adds a relation between this path and another path.
	 *
	 * @param OtherPath The other path to relate to this path.
	 */
	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	void AddPathRelation(ACarPath* OtherPath);

	/**
	 * Removes a relation between this path and another path.
	 *
	 * @param OtherPath The other path to remove from the relations.
	 */
	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	void RemovePathRelation(ACarPath* OtherPath);

	/**
	 * Updates the relations of this path with other paths.
	 * Ensures bidirectional relationships are maintained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	void UpdateRelations();
};
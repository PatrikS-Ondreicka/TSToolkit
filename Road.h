// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.generated.h"

/**
 * Enum representing the type of road.
 * - OneWay: A road with traffic flowing in one direction.
 * - TwoWay: A road with traffic flowing in both directions.
 */
UENUM()
enum class ERoadType
{
	OneWay,
	TwoWay
};

/**
 * ARoad represents a road in the simulation.
 * It includes a spline component to define the road's path and a static mesh for its appearance.
 */
UCLASS()
class TSTOOLKIT_API ARoad : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ARoad.
	 * Sets default values for this actor's properties.
	 */
	ARoad();

	/** Spline component defining the path of the road. */
	UPROPERTY(EditAnywhere, Category = "Road Components")
	class USplineComponent* RoadSpline;

	/** Static mesh used to visually represent the road. */
	UPROPERTY(EditAnywhere, Category = "Road Components")
	class UStaticMesh* RoadMesh;

	/** Type of the road (OneWay or TwoWay). */
	UPROPERTY(EditAnywhere, Category = "Road Details")
	ERoadType RoadType = ERoadType::OneWay;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to the road.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called when the actor is constructed or properties are changed in the editor.
	 * Sets up the road's mesh based on the spline and other properties.
	 *
	 * @param Transform The transform of the actor.
	 */
	void OnConstruction(const FTransform& Transform) override;

private:
	/**
	 * Sets up the road's mesh along the spline.
	 * This method is called during construction to align the mesh with the spline.
	 */
	void _SetUpMesh();
};

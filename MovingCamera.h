// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "Components/SplineComponent.h"
#include "MovingCamera.generated.h"

/**
 * AMovingCamera represents a camera that moves along a spline path.
 * It supports cyclic movement and allows customization of movement speed and behavior.
 */
UCLASS()
class TSTOOLKIT_API AMovingCamera : public ACamera
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for AMovingCamera.
	 * Initializes default values for the camera's properties.
	 */
	AMovingCamera();

	/** Spline component defining the path the camera will follow. */
	UPROPERTY(EditAnywhere, Category = "Camera components")
	USplineComponent* CameraPath;

	/** Whether the camera should move cyclically along the spline. */
	UPROPERTY(EditAnywhere, Category = "Camera movement")
	bool bCyclicMovement = false;

	/** Speed of the camera's movement along the spline, in units per second. */
	UPROPERTY(EditAnywhere, Category = "Camera movement")
	float MovementSpeed = 10.0f;

private:
	/** Current distance traveled along the spline. */
	float _CurrentDistance = 0.0f;

	/** Target distance for the camera to reach along the spline. */
	float _CurrentTargetDistance = 0.0f;

	/** Direction of movement along the spline (1 for forward, -1 for backward). */
	float _DistanceDirection = 1.0f;

	/** Tolerance for determining when the camera has reached its target distance. */
	float _DistanceTolerance = 50.0f;

public:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Called every frame to update the camera's position along the spline.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called when the actor is constructed or properties are changed in the editor.
	 * Updates the camera's position based on the spline.
	 *
	 * @param Transform The transform of the actor.
	 */
	void OnConstruction(const FTransform& Transform) override;

private:
	/**
	 * Moves the camera along the spline based on the specified speed and delta time.
	 *
	 * @param Speed The speed of movement along the spline.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	void _MoveAlongSpline(float Speed, float DeltaTime);

	/**
	 * Resets the camera's position to the start of the spline.
	 */
	void _ReturnAtSplineStart();

	/**
	 * Reverses the direction of the camera's movement along the spline.
	 */
	void _ReverseMovement();

	/**
	 * Sets the camera's location on the spline at the specified distance.
	 *
	 * @param Distance The distance along the spline to set the camera's location.
	 */
	void _SetLocationOnSpline(float Distance);
};

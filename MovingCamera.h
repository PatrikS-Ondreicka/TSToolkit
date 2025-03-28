// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "Components/SplineComponent.h"
#include "MovingCamera.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API AMovingCamera : public ACamera
{
	GENERATED_BODY()

public:
	AMovingCamera();

	UPROPERTY(EditAnywhere, Category = "Camera components")
	USplineComponent* CameraPath;

	UPROPERTY(EditAnywhere, Category = "Camera movement")
	bool bCyclicMovement = false;

	UPROPERTY(EditAnywhere, Category = "Camera movement")
	float MovementSpeed = 10.0f;

private:
	float _CurrentDistance = 0.0f;
	float _CurrentTargetDistance = 0.0f;
	float _DistanceDirection = 1.0f;
	float _DistanceTolerance = 50.0f;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnConstruction(const FTransform& Transform) override;

private:
	void _MoveAlongSpline(float Speed, float DeltaTime);
	void _ReturnAtSplineStart();
	void _ReverseMovement();
	void _SetLocationOnSpline(float Distance);
};

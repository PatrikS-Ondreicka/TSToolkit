// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingCamera.h"
#include "Camera/CameraComponent.h"

AMovingCamera::AMovingCamera()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraPath = CreateDefaultSubobject<USplineComponent>("Camera movement path");
	CameraPath->SetupAttachment(RootComponent);
}

void AMovingCamera::BeginPlay()
{
	_CurrentTargetDistance = CameraPath->GetSplineLength();
	_CurrentDistance = 0.0f;
	_DistanceTolerance = MovementSpeed / 10;
	_SetLocationOnSpline(_CurrentDistance);
}

void AMovingCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_MoveAlongSpline(MovementSpeed, DeltaTime);
}

void AMovingCamera::OnConstruction(const FTransform& Transform)
{
	_SetLocationOnSpline(_CurrentDistance);
}

void AMovingCamera::_MoveAlongSpline(float Speed, float DeltaTime)
{
	_CurrentDistance += (_DistanceDirection * (Speed * DeltaTime));
	_SetLocationOnSpline(_CurrentDistance);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Distance %f, Target % f"), _CurrentDistance, _CurrentTargetDistance));
	if (!FMath::IsNearlyEqual(_CurrentDistance, _CurrentTargetDistance, _DistanceTolerance))
	{
		return;
	}

	if (bCyclicMovement)
	{
		_ReturnAtSplineStart();
	}
	else
	{
		_ReverseMovement();
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Switch %f, Target % f"), _CurrentDistance, _CurrentTargetDistance));
}

void AMovingCamera::_ReturnAtSplineStart()
{
	_CurrentDistance = 0.0f;
	_SetLocationOnSpline(_CurrentDistance);
}

void AMovingCamera::_ReverseMovement()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reverse movement"));
	_DistanceDirection *= -1;
	_CurrentTargetDistance = (_DistanceDirection < 0)
		? 0.0f
		: CameraPath->GetSplineLength();
}

void AMovingCamera::_SetLocationOnSpline(float Distance)
{
	UCameraComponent* camera = GetCameraComponent();
	if (!camera)
	{
		return;
	}
	FVector location = CameraPath->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	camera->SetWorldLocation(location, false);
}

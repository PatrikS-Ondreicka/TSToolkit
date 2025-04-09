// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingCamera.h"
#include "Camera/CameraComponent.h"

/**
 * Constructor for AMovingCamera.
 * Initializes the spline component and sets default properties for the camera.
 */
AMovingCamera::AMovingCamera()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach the spline component
	CameraPath = CreateDefaultSubobject<USplineComponent>(TEXT("Camera movement path"));
	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CameraPath in AMovingCamera constructor."));
	}
	CameraPath->SetupAttachment(RootComponent);
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes the camera's position and movement parameters.
 */
void AMovingCamera::BeginPlay()
{
	Super::BeginPlay();

	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in BeginPlay."));
		return;
	}

	_CurrentTargetDistance = CameraPath->GetSplineLength();
	_CurrentDistance = 0.0f;
	_DistanceTolerance = MovementSpeed / 10;
	_SetLocationOnSpline(_CurrentDistance);
}

/**
 * Called every frame to update the camera's position along the spline.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AMovingCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in Tick."));
		return;
	}

	_MoveAlongSpline(MovementSpeed, DeltaTime);
}

/**
 * Called when the actor is constructed or properties are changed in the editor.
 * Updates the camera's position based on the spline.
 *
 * @param Transform The transform of the actor.
 */
void AMovingCamera::OnConstruction(const FTransform& Transform)
{
	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in OnConstruction."));
		return;
	}

	_SetLocationOnSpline(_CurrentDistance);
}

/**
 * Moves the camera along the spline based on the specified speed and delta time.
 *
 * @param Speed The speed of movement along the spline.
 * @param DeltaTime The time elapsed since the last frame.
 */
void AMovingCamera::_MoveAlongSpline(float Speed, float DeltaTime)
{
	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in _MoveAlongSpline."));
		return;
	}

	_CurrentDistance += (_DistanceDirection * (Speed * DeltaTime));
	_SetLocationOnSpline(_CurrentDistance);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Distance: %f, Target: %f"), _CurrentDistance, _CurrentTargetDistance));
	}

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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Switch: %f, Target: %f"), _CurrentDistance, _CurrentTargetDistance));
	}
}

/**
 * Resets the camera's position to the start of the spline.
 */
void AMovingCamera::_ReturnAtSplineStart()
{
	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in _ReturnAtSplineStart."));
		return;
	}

	_CurrentDistance = 0.0f;
	_SetLocationOnSpline(_CurrentDistance);
}

/**
 * Reverses the direction of the camera's movement along the spline.
 */
void AMovingCamera::_ReverseMovement()
{
	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in _ReverseMovement."));
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reverse movement"));
	}

	_DistanceDirection *= -1;
	_CurrentTargetDistance = (_DistanceDirection < 0)
		? 0.0f
		: CameraPath->GetSplineLength();
}

/**
 * Sets the camera's location on the spline at the specified distance.
 *
 * @param Distance The distance along the spline to set the camera's location.
 */
void AMovingCamera::_SetLocationOnSpline(float Distance)
{
	if (!CameraPath)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraPath is null in _SetLocationOnSpline."));
		return;
	}

	UCameraComponent* camera = GetCameraComponent();
	if (!camera)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraComponent is null in _SetLocationOnSpline."));
		return;
	}

	FVector location = CameraPath->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	camera->SetWorldLocation(location, false);
}

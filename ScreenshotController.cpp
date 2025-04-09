// Fill out your copyright notice in the Description page of Project Settings.

#include "ScreenshotController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameEngine.h"
#include "Camera.h"

/**
 * Constructor for AScreenshotController.
 * Initializes default values for the screenshot controller's properties.
 */
AScreenshotController::AScreenshotController()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes cameras and resets internal values for screenshot tracking.
 */
void AScreenshotController::BeginPlay()
{
	Super::BeginPlay();

	if (bRegisterAllAtBeginPlay)
	{
		_RegisterAllCameras();
	}

	_ResetScreenshotValues();
	_ResetTimer();
}

/**
 * Resets internal values related to screenshot tracking.
 */
void AScreenshotController::_ResetScreenshotValues()
{
	_CurrentCamera = nullptr;
	_CurrentCameraIndex = -1;
	_CurrentCameraCountdown = 0;
	_ScreenshotsTakenCount = 0;
}

/**
 * Handles actions to be performed when the screenshot timer runs out.
 * Sets the _TimerRunOut flag to true.
 */
void AScreenshotController::_TimerAction()
{
	_TimerRunOut = true;
}

/**
 * Resets the screenshot timer to the configured interval.
 */
void AScreenshotController::_ResetTimer()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _ResetTimer."));
		return;
	}

	FTimerHandle timerHandle;
	_TimerRunOut = false;
	GetWorldTimerManager().SetTimer(timerHandle, this, &AScreenshotController::_TimerAction, ScreenshotInterval, false);
}

/**
 * Called every frame to update the actor.
 * Handles the logic for taking screenshots at configured intervals.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AScreenshotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Cameras.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No cameras registered in Tick."));
		return;
	}

	if (_TimerRunOut)
	{
		_CurrentCameraCountdown -= DeltaTime;

		if (_ScreenshotsTakenCount == Cameras.Num())
		{
			_ResetScreenshotValues();
			_ResetTimer();
			return;
		}

		if (_CurrentCameraCountdown <= 0)
		{
			_CurrentCameraIndex++;
			if (Cameras.IsValidIndex(_CurrentCameraIndex))
			{
				_CurrentCamera = Cameras[_CurrentCameraIndex];
				if (!_CurrentCamera)
				{
					UE_LOG(LogTemp, Warning, TEXT("Current camera is null in Tick."));
					return;
				}

				_CurrentCameraCountdown = DelayBetweenScreenshots;
				_CurrentCamera->TakeScreenshot();
				_ScreenshotsTakenCount++;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid camera index in Tick."));
			}
		}
	}
}

/**
 * Registers all cameras in the simulation.
 * This method is called if bRegisterAllAtBeginPlay is true.
 */
void AScreenshotController::_RegisterAllCameras()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _RegisterAllCameras."));
		return;
	}

	Cameras.Empty();

	TArray<AActor*> found;
	UGameplayStatics::GetAllActorsOfClass(world, ACamera::StaticClass(), found);

	for (AActor* actor : found)
	{
		if (!actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Null actor found in _RegisterAllCameras."));
			continue;
		}

		ACamera* camera = Cast<ACamera>(actor);
		if (camera)
		{
			Cameras.Add(camera);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast actor to ACamera in _RegisterAllCameras."));
		}
	}
}

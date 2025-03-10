// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenshotController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameEngine.h"
#include "PeriodicTimer.h"
#include "Camera.h"

// Sets default values
AScreenshotController::AScreenshotController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_timer = NewObject<UPeriodicTimer>();
	_currentCamera = nullptr;
	_currentCameraIndex = -1;
	_currentCameraCountdown = 0;
}

// Called when the game starts or when spawned
void AScreenshotController::BeginPlay()
{
	Super::BeginPlay();
	_timer->SetInitValue(ScreenshotInterval);
	if (RegisterAllAtBeginPlay)
	{
		_registerAllCameras();
	}
}

// Called every frame
void AScreenshotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Cameras.Num() < 0)
	{
		return;
	}

	if (_timer->CoundownState())
	{

		_currentCameraCountdown -= DeltaTime;

		if (_screenshotsTakenCount == Cameras.Num())
		{
			_timer->ResetCountdown();
			_currentCamera = nullptr;
			_currentCameraIndex = -1;
			_currentCameraCountdown = 0;
			_screenshotsTakenCount = 0;
			return;
		}

		if (_currentCameraCountdown <= 0)
		{
			_currentCameraIndex++;
			if (Cameras.IsValidIndex(_currentCameraIndex))
			{
				_currentCamera = Cameras[_currentCameraIndex];
				_currentCameraCountdown = DelayBetweenScreenshots;
				_currentCamera->TakeScreenshot();
				_screenshotsTakenCount++;
			}

		}
	}
	else
	{
		_timer->DecrementCountdown(DeltaTime);
	}
}

void AScreenshotController::_registerAllCameras()
{
	UWorld* world = GetWorld();
	Cameras.Empty();

	TArray<AActor*> found;

	UGameplayStatics::GetAllActorsOfClass(world, ACamera::StaticClass(), found);
	for (AActor* actor : found)
	{
		ACamera* camera = Cast<ACamera>(actor);
		if (camera)
		{
			Cameras.Add(camera);
		}
	}
}


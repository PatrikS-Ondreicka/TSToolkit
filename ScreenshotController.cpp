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
	_Timer = NewObject<UPeriodicTimer>();
	_CurrentCamera = nullptr;
	_CurrentCameraIndex = -1;
	_CurrentCameraCountdown = 0;
	_ScreenshotsTakenCount = 0;
}

// Called when the game starts or when spawned
void AScreenshotController::BeginPlay()
{
	Super::BeginPlay();
	_Timer->SetInitValue(ScreenshotInterval);
	if (bRegisterAllAtBeginPlay)
	{
		_RegisterAllCameras();
	}
}

// Called every frame
void AScreenshotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Cameras.Num() <= 0)
	{
		return;
	}

	if (_Timer->CountdownState())
	{
		_CurrentCameraCountdown -= DeltaTime;

		if (_ScreenshotsTakenCount == Cameras.Num())
		{
			_Timer->ResetCountdown();
			_CurrentCamera = nullptr;
			_CurrentCameraIndex = -1;
			_CurrentCameraCountdown = 0;
			_ScreenshotsTakenCount = 0;
			return;
		}

		if (_CurrentCameraCountdown <= 0)
		{
			_CurrentCameraIndex++;
			if (Cameras.IsValidIndex(_CurrentCameraIndex))
			{
				_CurrentCamera = Cameras[_CurrentCameraIndex];
				_CurrentCameraCountdown = DelayBetweenScreenshots;
				_CurrentCamera->TakeScreenshot();
				_ScreenshotsTakenCount++;
			}
		}
	}
	else
	{
		_Timer->DecrementCountdown(DeltaTime);
	}
}

void AScreenshotController::_RegisterAllCameras()
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
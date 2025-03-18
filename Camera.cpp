// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera.h"
#include "HighResScreenshot.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#define MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(str));
#define ERROR_MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(str));

ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;
	UCameraComponent* cameraComponent = GetCameraComponent();

	cameraComponent->AspectRatio = 16.0f / 9.0f;
	cameraComponent->bConstrainAspectRatio = false;

	SaveDirectory = FPaths::ProjectDir() + "Screenshots/" + CameraName + "/";
	_ScreenshotCountdown = ScreenshotInterval;
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AutomaticScreenshots)
	{
		_AutoAction(DeltaTime);
	}
}

void ACamera::OnConstruction(const FTransform& Transform)
{
	SaveDirectory = FPaths::ProjectDir() + "Screenshots/" + CameraName + "/";
}

void ACamera::TakeScreenshot()
{
	// Creation of filepath
	FDateTime currentTime = FDateTime::Now();
	FString currentTimeString = currentTime.ToString(TEXT("%Y%m%d%H%M%S"));
	FString filename = currentTimeString + ".png";
	FString filepath = FPaths::Combine(SaveDirectory, filename);

	// Store original view port
	UWorld* world = GetWorld();
	APlayerController* playerController = UGameplayStatics::GetPlayerController(world, 0);
	AActor* originalView = playerController->GetViewTarget();

	// Switch to that of a camera
	UCameraComponent* cameraComponent = GetCameraComponent();
	playerController->SetViewTarget(this);

	// Screenshot settings
	FHighResScreenshotConfig* screenshotConfig = &GetHighResScreenshotConfig();
	screenshotConfig->ResolutionMultiplier = 15;
	screenshotConfig->bCaptureHDR = false;
	screenshotConfig->bMaskEnabled = false;

	// Take screenshot
	FScreenshotRequest::RequestScreenshot(filepath, false, false);
}

void ACamera::_AutoAction(float deltaTime)
{
	_ScreenshotCountdown -= deltaTime;
	if (_ScreenshotCountdown <= 0)
	{
		_ScreenshotCountdown = ScreenshotInterval;
		TakeScreenshot();
	}
}
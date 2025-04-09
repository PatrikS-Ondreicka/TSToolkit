#include "Camera.h"
#include "HighResScreenshot.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

/**
 * Constructor for the ACamera class.
 * Initializes the camera component and sets default values for properties.
 */
ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;
	UCameraComponent* cameraComponent = GetCameraComponent();

	if (!cameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraComponent is null in ACamera constructor."));
		return;
	}

	// Set default aspect ratio and constraints
	cameraComponent->AspectRatio = 16.0f / 9.0f;
	cameraComponent->bConstrainAspectRatio = false;

	// Set default save directory and screenshot countdown
	SaveDirectory = FPaths::ProjectDir() + "Screenshots/" + CameraName + "/";
	_ScreenshotCountdown = ScreenshotInterval;
}

/**
 * Called every frame to update the camera.
 * Handles automatic screenshot functionality if enabled.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AutomaticScreenshots)
	{
		_AutoAction(DeltaTime);
	}
}

/**
 * Called when the actor is constructed or properties are changed in the editor.
 * Updates the save directory based on the camera name.
 *
 * @param Transform The transform of the actor.
 */
void ACamera::OnConstruction(const FTransform& Transform)
{
	SaveDirectory = FPaths::ProjectDir() + "Screenshots/" + CameraName + "/";
}

/**
 * Takes a screenshot and saves it to the specified directory.
 * Configures screenshot settings and temporarily switches the view target to this camera.
 */
void ACamera::TakeScreenshot()
{
	// Create the file path for the screenshot
	FDateTime currentTime = FDateTime::Now();
	FString currentTimeString = currentTime.ToString(TEXT("%Y%m%d%H%M%S"));
	FString filename = currentTimeString + ".png";
	FString filepath = FPaths::Combine(SaveDirectory, filename);

	// Store the original view target
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in TakeScreenshot."));
		return;
	}

	APlayerController* playerController = UGameplayStatics::GetPlayerController(world, 0);
	if (!playerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null in TakeScreenshot."));
		return;
	}

	AActor* originalView = playerController->GetViewTarget();
	if (!originalView)
	{
		UE_LOG(LogTemp, Error, TEXT("Original view target is null in TakeScreenshot."));
		return;
	}

	// Switch the view target to this camera
	UCameraComponent* cameraComponent = GetCameraComponent();
	if (!cameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraComponent is null in TakeScreenshot."));
		return;
	}
	playerController->SetViewTarget(this);

	// Configure screenshot settings
	FHighResScreenshotConfig* screenshotConfig = &GetHighResScreenshotConfig();
	if (!screenshotConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("ScreenshotConfig is null in TakeScreenshot."));
		return;
	}
	screenshotConfig->ResolutionMultiplier = 15;
	screenshotConfig->bCaptureHDR = false;
	screenshotConfig->bMaskEnabled = false;

	// Request the screenshot
	FScreenshotRequest::RequestScreenshot(filepath, false, false);
}

/**
 * Handles automatic actions such as taking screenshots.
 * Decrements the countdown timer and triggers a screenshot when the timer reaches zero.
 *
 * @param deltaTime The time elapsed since the last frame.
 */
void ACamera::_AutoAction(float deltaTime)
{
	_ScreenshotCountdown -= deltaTime;
	if (_ScreenshotCountdown <= 0)
	{
		_ScreenshotCountdown = ScreenshotInterval;
		TakeScreenshot();
	}
}
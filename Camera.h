// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Camera.generated.h"

/**
 * ACamera is a custom camera actor that supports automatic screenshots
 * and allows configuration of screenshot intervals and save directories.
 */
UCLASS()
class TSTOOLKIT_API ACamera : public ACameraActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for the ACamera class.
	 */
	ACamera();

	/** The name of the camera. */
	UPROPERTY(EditAnywhere, Category = "Camera details")
	FString CameraName = "Default";

	/** Whether the camera should automatically take screenshots. */
	UPROPERTY(EditAnywhere, Category = "Camera details")
	bool AutomaticScreenshots = false;

	/** The interval (in seconds) between automatic screenshots. */
	UPROPERTY(EditAnywhere, Category = "Screenshot details")
	float ScreenshotInterval = 10.0f;

	/** The directory where screenshots will be saved. */
	UPROPERTY(EditAnywhere, Category = "Screenshot details")
	FString SaveDirectory;

private:
	/** Internal countdown timer for automatic screenshots. */
	float _ScreenshotCountdown;

public:
	/**
	 * Called every frame to update the camera.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called when the actor is constructed or properties are changed in the editor.
	 * @param Transform The transform of the actor.
	 */
	void OnConstruction(const FTransform& Transform) override;

	/**
	 * Takes a screenshot and saves it to the specified directory.
	 */
	UFUNCTION()
	void TakeScreenshot();

protected:
	/**
	 * Handles automatic actions such as taking screenshots.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	void _AutoAction(float DeltaTime);
};
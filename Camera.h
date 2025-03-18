// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Camera.generated.h"

/**
 *
 */
UCLASS()
class TSTOOLKIT_API ACamera : public ACameraActor
{
	GENERATED_BODY()

public:
	ACamera();

	UPROPERTY(EditAnywhere, Category = "Camera details")
	FString CameraName = "Default";

	UPROPERTY(EditAnywhere, Category = "Camera details")
	bool AutomaticScreenshots = false;

	UPROPERTY(EditAnywhere, Category = "Screenshot details")
	float ScreenshotInterval = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Screenshot details")
	FString SaveDirectory;

private:
	float _ScreenshotCountdown;

public:
	virtual void Tick(float DeltaTime) override;
	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void TakeScreenshot();

private:
	void _AutoAction(float DeltaTime);
};
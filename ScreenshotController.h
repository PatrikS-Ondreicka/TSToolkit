// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScreenshotController.generated.h"

class ACamera;

/**
 * AScreenshotController is responsible for managing cameras and taking periodic screenshots.
 * It supports multiple cameras and allows configuration of intervals and delays between screenshots.
 */
UCLASS()
class TSTOOLKIT_API AScreenshotController : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for AScreenshotController.
	 * Sets default values for this actor's properties.
	 */
	AScreenshotController();

	/** Array of cameras managed by this controller. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	TArray<ACamera*> Cameras;

	/** Whether to automatically register all cameras at BeginPlay. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool bRegisterAllAtBeginPlay = true;

	/** Interval between screenshots, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float ScreenshotInterval = 10.0f;

	/** Delay between consecutive screenshots, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float DelayBetweenScreenshots = 0.1f;

private:
	/** Countdown timer for the current camera's screenshot interval. */
	float _CurrentCameraCountdown;

	/** Total number of screenshots taken. */
	int _ScreenshotsTakenCount;

	/** Index of the currently active camera in the Cameras array. */
	int _CurrentCameraIndex;

	/** Pointer to the currently active camera. */
	ACamera* _CurrentCamera;

	/** Indicates whether the screenshot timer has run out. */
	bool _TimerRunOut = false;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * Handles actions to be performed when the screenshot timer runs out.
	 */
	virtual void _TimerAction();

	/**
	 * Resets the screenshot timer to the configured interval.
	 */
	virtual void _ResetTimer();

	/**
	 * Resets internal values related to screenshot tracking.
	 */
	virtual void _ResetScreenshotValues();

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

private:
	/**
	 * Registers all cameras in the simulation.
	 * This method is called if bRegisterAllAtBeginPlay is true.
	 */
	void _RegisterAllCameras();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScreenshotController.generated.h"

class ACamera;

UCLASS()
class TSTOOLKIT_API AScreenshotController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AScreenshotController();

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	TArray<ACamera*> Cameras;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool bRegisterAllAtBeginPlay = true;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float ScreenshotInterval = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float DelayBetweenScreenshots = 0.1f;

private:
	float _CurrentCameraCountdown;
	int _ScreenshotsTakenCount;
	int _CurrentCameraIndex;
	ACamera* _CurrentCamera;
	bool _TimerRunOut = false;

protected:
	virtual void BeginPlay() override;
	virtual void _TimerAction();
	virtual void _ResetTimer();
	virtual void _ResetScreenshotValues();

public:
	virtual void Tick(float DeltaTime) override;

private:
	void _RegisterAllCameras();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimConfig.h"
#include "GameFramework/GameModeBase.h"
#include "WeatherController.h"
#include "TSToolkitGameMode.generated.h"

/**
 *
 */
UCLASS()
class TSTOOLKIT_API ATSToolkitGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATSToolkitGameMode();

	// Main menu details
	UPROPERTY(VisibleAnywhere, Category = "MainMenuDetails")
	FString MainMenuLevelName = "MainMenu";

	UPROPERTY(VisibleAnywhere, Category = "MainMenuDetails")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "WeatherControllerDetails")
	TSubclassOf<AWeatherController> WeatherControllerClass;

public:
	virtual void Tick(float DeltaTime) override;
	
	bool IsMainMenu() const;
	void LoadMainMenu();
	void LoadLevel(USimConfig* Config);

protected:
	virtual void BeginPlay() override;
	virtual void _EndLevel();

private:
	void _UIViewportSetup(UUserWidget* Widget);
	void _LevelViewportSetup();

	// Level setup functions
	void _SetUpLevel(USimConfig* Config);
	void _SetUpCarSpawnController(USimConfig* Config);
	void _SetUpScreenshotController(USimConfig* Config);
	void _SetUpWeatherController(USimConfig* Config);
};


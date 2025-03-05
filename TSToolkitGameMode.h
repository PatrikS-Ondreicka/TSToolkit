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
	UPROPERTY(VisibleAnywhere, Category = "Main menu details")
	FString MainMenuLevelName = "MainMenu";

	UPROPERTY(VisibleAnywhere, Category = "Main menu details")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Weather controller details")
	TSubclassOf<AWeatherController> WeatherControllerClass;

public:
	virtual void BeginPlay() override;
	bool IsMainMenu() const;
	void LoadMainMenu();
	void LoadLevel(USimConfig* config);

private:
	void _UIViewportSetup(UUserWidget* widget);
	void _levelVieportSetup();

	// level setup functions
	void _setUpLevel(USimConfig* config);
	void _setUpCarSpawnController(USimConfig* config);
	void _setUpScreenshotController(USimConfig* config);
	void _setUpWeatherController(USimConfig* config);
};

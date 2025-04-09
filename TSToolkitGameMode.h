// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimConfig.h"
#include "GameFramework/GameModeBase.h"
#include "WeatherController.h"
#include "TSToolkitGameMode.generated.h"

/**
 * ATSToolkitGameMode is the main game mode class for the simulation.
 * It handles level loading, main menu setup, and configuration of various controllers.
 */
UCLASS()
class TSTOOLKIT_API ATSToolkitGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ATSToolkitGameMode.
	 * Initializes default values for the game mode.
	 */
	ATSToolkitGameMode();

	/** Name of the main menu level. */
	UPROPERTY(VisibleAnywhere, Category = "MainMenuDetails")
	FString MainMenuLevelName = "MainMenu";

	/** Class of the main menu widget to display. */
	UPROPERTY(VisibleAnywhere, Category = "MainMenuDetails")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	/** Class of the weather controller to use in the simulation. */
	UPROPERTY(VisibleAnywhere, Category = "WeatherControllerDetails")
	TSubclassOf<AWeatherController> WeatherControllerClass;

public:
	/**
	 * Called every frame to update the game mode.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Checks if the current level is the main menu.
	 *
	 * @return True if the current level is the main menu, false otherwise.
	 */
	bool IsMainMenu() const;

	/**
	 * Loads the main menu level.
	 */
	void LoadMainMenu();

	/**
	 * Loads a level based on the provided simulation configuration.
	 *
	 * @param Config The simulation configuration to use for loading the level.
	 */
	void LoadLevel(USimConfig* Config);

protected:
	/**
	 * Called when the game starts or when the game mode is initialized.
	 * Used for initialization logic specific to the game mode.
	 */
	virtual void BeginPlay() override;

	/**
	 * Ends the current level and performs cleanup.
	 */
	virtual void _EndLevel();

private:
	/**
	 * Sets up the UI viewport with the specified widget.
	 *
	 * @param Widget The widget to display in the viewport.
	 */
	void _UIViewportSetup(UUserWidget* Widget);

	/**
	 * Sets up the viewport for the level.
	 */
	void _LevelViewportSetup();

	// Level setup functions

	/**
	 * Sets up the level based on the provided simulation configuration.
	 *
	 * @param Config The simulation configuration to use for setting up the level.
	 */
	void _SetUpLevel(USimConfig* Config);

	/**
	 * Sets up the car spawn controller based on the provided simulation configuration.
	 *
	 * @param Config The simulation configuration to use for setting up the car spawn controller.
	 */
	void _SetUpCarSpawnController(USimConfig* Config);

	/**
	 * Sets up the screenshot controller based on the provided simulation configuration.
	 *
	 * @param Config The simulation configuration to use for setting up the screenshot controller.
	 */
	void _SetUpScreenshotController(USimConfig* Config);

	/**
	 * Sets up the weather controller based on the provided simulation configuration.
	 *
	 * @param Config The simulation configuration to use for setting up the weather controller.
	 */
	void _SetUpWeatherController(USimConfig* Config);
};

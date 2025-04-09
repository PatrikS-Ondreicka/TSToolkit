// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimConfig.h"
#include "MainMenu.generated.h"

/**
 * UMainMenu represents the main menu of the simulation.
 * It provides functionality to configure simulation settings and start the simulation.
 */
UCLASS()
class TSTOOLKIT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for UMainMenu.
	 * Initializes the main menu widget.
	 *
	 * @param ObjectInitializer The object initializer for the widget.
	 */
	UMainMenu(const FObjectInitializer& ObjectInitializer);

private:
	/** Pointer to the simulation configuration object. */
	USimConfig* _SimConfig;

public:
	// Event handlers

	/**
	 * Handles the Start button click event.
	 * Starts the simulation with the configured settings.
	 */
	UFUNCTION(BlueprintCallable)
	void StartButtonClick();

	// Getters

	/**
	 * Gets the relative path of the level to load.
	 *
	 * @return The relative level path as a string.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetRelativeLevelPath() const
	{
		return _SimConfig->RelativeLevelPath;
	}

	/**
	 * Gets the duration of the simulation.
	 *
	 * @return The simulation duration in seconds.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSimulationDuration() const
	{
		return _SimConfig->SimulationDuration;
	}

	/**
	 * Gets the car spawn controller class name.
	 *
	 * @return The car spawn controller class name as an enum.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE ECarSpawnControllerClasses GetCarSpawnControllerClassName() const
	{
		return _SimConfig->ControllerClassName;
	}

	/**
	 * Gets the car spawn controller class name as a string.
	 *
	 * @return The car spawn controller class name as a string.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetCarSpawnControllerClassNameAsString() const
	{
		return USimConfig::GetCarSpawnControllerClassString(_SimConfig->ControllerClassName);
	}

	/**
	 * Gets the car spawn rate.
	 *
	 * @return The car spawn rate in cars per second.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCarsSpawnRate() const
	{
		return _SimConfig->CarsSpawnRate;
	}

	/**
	 * Gets the screenshot interval.
	 *
	 * @return The interval between screenshots in seconds.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetScreenshotInterval() const
	{
		return _SimConfig->ScreenshotInterval;
	}

	/**
	 * Gets the delay between screenshots.
	 *
	 * @return The delay between screenshots in seconds.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDelayBetweenScreenshots() const
	{
		return _SimConfig->DelayBetweenScreenshots;
	}

	/**
	 * Checks if the simulation starts at night.
	 *
	 * @return True if the simulation starts at night, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsNight() const
	{
		return _SimConfig->bIsNight;
	}

	/**
	 * Checks if the simulation starts with overcast weather.
	 *
	 * @return True if the simulation starts with overcast weather, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsOvercast() const
	{
		return _SimConfig->bIsOvercast;
	}

	/**
	 * Checks if the simulation starts with rain.
	 *
	 * @return True if the simulation starts with rain, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsRain() const
	{
		return _SimConfig->bIsRain;
	}

	/**
	 * Checks if the simulation changes day and night dynamically.
	 *
	 * @return True if the simulation changes day and night dynamically, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsChangeDayTime() const
	{
		return _SimConfig->bIsChangeDayTime;
	}

	/**
	 * Gets the rate of day and night changes.
	 *
	 * @return The rate of day and night changes in seconds.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetChangeDayTimeRate() const
	{
		return _SimConfig->ChangeDayTimeRate;
	}

	/**
	 * Checks if the simulation changes overcast weather dynamically.
	 *
	 * @return True if the simulation changes overcast weather dynamically, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsChangeOvercast() const
	{
		return _SimConfig->bIsChangeOvercast;
	}

	/**
	 * Gets the rate of overcast weather changes.
	 *
	 * @return The rate of overcast weather changes in seconds.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetChangeOvercastRate() const
	{
		return _SimConfig->ChangeOvercastRate;
	}

	/**
	 * Checks if the simulation changes rain dynamically.
	 *
	 * @return True if the simulation changes rain dynamically, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsChangeRain() const
	{
		return _SimConfig->bIsChangeRain;
	}

	/**
	 * Gets the rate of rain changes.
	 *
	 * @return The rate of rain changes in seconds.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetChangeRainRate() const
	{
		return _SimConfig->ChangeRainRate;
	}

	// Setters

	/**
	 * Sets the relative path of the level to load.
	 *
	 * @param Value The relative level path as a string.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetRelativeLevelPath(const FString& Value)
	{
		_SimConfig->RelativeLevelPath = Value;
	}

	/**
	 * Sets the duration of the simulation.
	 *
	 * @param Value The simulation duration in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSimulationDuration(float Value)
	{
		_SimConfig->SimulationDuration = Value;
	}

	/**
	 * Sets the car spawn controller class name.
	 *
	 * @param Value The car spawn controller class name as an enum.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarSpawnControllerClassName(ECarSpawnControllerClasses Value)
	{
		_SimConfig->ControllerClassName = Value;
	}

	/**
	 * Sets the car spawn controller class name by string.
	 *
	 * @param Value The car spawn controller class name as a string.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarSpawnControllerClassNameByString(const FString& Value)
	{
		_SimConfig->ControllerClassName = USimConfig::GetCarSpawnControllerClassByName(Value);
	}

	/**
	 * Sets the car spawn rate.
	 *
	 * @param Value The car spawn rate in cars per second.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarsSpawnRate(float Value)
	{
		_SimConfig->CarsSpawnRate = Value;
	}

	/**
	 * Sets the screenshot interval.
	 *
	 * @param Value The interval between screenshots in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetScreenshotInterval(float Value)
	{
		_SimConfig->ScreenshotInterval = Value;
	}

	/**
	 * Sets the delay between screenshots.
	 *
	 * @param Value The delay between screenshots in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDelayBetweenScreenshots(float Value)
	{
		_SimConfig->DelayBetweenScreenshots = Value;
	}

	/**
	 * Sets whether the simulation starts at night.
	 *
	 * @param Value True if the simulation starts at night, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsNight(bool Value)
	{
		_SimConfig->bIsNight = Value;
	}

	/**
	 * Sets whether the simulation starts with overcast weather.
	 *
	 * @param Value True if the simulation starts with overcast weather, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsOvercast(bool Value)
	{
		_SimConfig->bIsOvercast = Value;
	}

	/**
	 * Sets whether the simulation starts with rain.
	 *
	 * @param Value True if the simulation starts with rain, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsRain(bool Value)
	{
		_SimConfig->bIsRain = Value;
	}

	/**
	 * Sets whether the simulation changes day and night dynamically.
	 *
	 * @param Value True if the simulation changes day and night dynamically, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsChangeDayTime(bool Value)
	{
		_SimConfig->bIsChangeDayTime = Value;
	}

	/**
	 * Sets the rate of day and night changes.
	 *
	 * @param Value The rate of day and night changes in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetChangeDayTimeRate(float Value)
	{
		_SimConfig->ChangeDayTimeRate = Value;
	}

	/**
	 * Sets whether the simulation changes overcast weather dynamically.
	 *
	 * @param Value True if the simulation changes overcast weather dynamically, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsChangeOvercast(bool Value)
	{
		_SimConfig->bIsChangeOvercast = Value;
	}

	/**
	 * Sets the rate of overcast weather changes.
	 *
	 * @param Value The rate of overcast weather changes in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetChangeOvercastRate(float Value)
	{
		_SimConfig->ChangeOvercastRate = Value;
	}

	/**
	 * Sets whether the simulation changes rain dynamically.
	 *
	 * @param Value True if the simulation changes rain dynamically, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsChangeRain(bool Value)
	{
		_SimConfig->bIsChangeRain = Value;
	}

	/**
	 * Sets the rate of rain changes.
	 *
	 * @param Value The rate of rain changes in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetChangeRainRate(float Value)
	{
		_SimConfig->ChangeRainRate = Value;
	}

	// Additional functions

	/**
	 * Gets a list of available levels.
	 *
	 * @return An array of level names as strings.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FString> GetLevels() const;

	/**
	 * Gets a list of available car spawn controller classes.
	 *
	 * @return An array of car spawn controller class names as strings.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FString> GetCarSpawnControllerClasses() const;

	/**
	 * Saves the current simulation configuration.
	 */
	UFUNCTION(BlueprintCallable)
	void SaveConfig();
};

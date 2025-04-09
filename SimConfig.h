// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CarSpawnController.h"
#include "SimConfig.generated.h"

/**
 * Enum representing the available car spawn controller classes.
 * - Random: Spawns cars at random intervals and locations.
 * - Periodic: Spawns cars at fixed intervals.
 */
UENUM()
enum class ECarSpawnControllerClasses
{
	Random,
	Periodic
};

/**
 * USimConfig is responsible for managing simulation configuration settings.
 * It provides functionality to save, load, and retrieve configuration details
 * for levels, car spawning, screenshots, and weather.
 */
UCLASS()
class TSTOOLKIT_API USimConfig : public UObject
{
	GENERATED_BODY()

private:
	/** Name of the main menu level. */
	static const FString _MainMenuLevelName;

public:
	/**
	 * Default constructor for USimConfig.
	 * Initializes default values for the simulation configuration.
	 */
	USimConfig();

	// File details
	/** Name of the configuration file. */
	static const FString ConfigFileName;

	/** Directory path where the configuration file is stored. */
	static const FString ConfigDirPath;

	// Level details
	/** Directory path where levels are stored. */
	static const FString LevelDirPath;

	/** Relative path of the level to load. */
	UPROPERTY(EditAnywhere, Category = "Level Details")
	FString RelativeLevelPath;

	// Simulation global details
	/** Duration of the simulation in seconds. */
	UPROPERTY(EditAnywhere, Category = "Simulation Details")
	float SimulationDuration;

	// Car spawn details
	/** Class name of the car spawn controller. */
	UPROPERTY(EditAnywhere, Category = "Car Spawning Details")
	ECarSpawnControllerClasses ControllerClassName;

	/** Rate at which cars are spawned, in cars per second. */
	UPROPERTY(EditAnywhere, Category = "Car Spawning Details")
	float CarsSpawnRate;

	// Screenshot details
	/** Interval between screenshots, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Screenshot Details")
	float ScreenshotInterval;

	/** Delay between consecutive screenshots, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Screenshot Details")
	float DelayBetweenScreenshots;

	// Weather details
	/** Whether the simulation starts at night. */
	UPROPERTY(EditAnywhere, Category = "Weather Details")
	bool bIsNight;

	/** Whether the simulation starts with overcast weather. */
	UPROPERTY(EditAnywhere, Category = "Weather Details")
	bool bIsOvercast;

	/** Whether the simulation starts with rain. */
	UPROPERTY(EditAnywhere, Category = "Weather Details")
	bool bIsRain;

	// Weather change details
	/** Whether the simulation dynamically changes day and night. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	bool bIsChangeDayTime;

	/** Rate of day and night changes, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	float ChangeDayTimeRate;

	/** Whether the simulation dynamically changes overcast weather. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	bool bIsChangeOvercast;

	/** Rate of overcast weather changes, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeOvercastRate;

	/** Whether the simulation dynamically changes rain. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	bool bIsChangeRain;

	/** Rate of rain changes, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeRainRate;

	// Static methods
public:
	/**
	 * Retrieves the names of all available levels.
	 *
	 * @return An array of level names as strings.
	 */
	static TArray<FString> GetLevelNames();

	/**
	 * Retrieves the names of all available car spawn controller classes.
	 *
	 * @return An array of car spawn controller class names as strings.
	 */
	static TArray<FString> GetCarSpawnControllerClassesNames();

	/**
	 * Retrieves the car spawn controller class enum value by its name.
	 *
	 * @param name The name of the car spawn controller class.
	 * @return The corresponding ECarSpawnControllerClasses enum value.
	 */
	static ECarSpawnControllerClasses GetCarSpawnControllerClassByName(FString name);

	/**
	 * Retrieves the name of a car spawn controller class as a string.
	 *
	 * @param className The car spawn controller class enum value.
	 * @return The corresponding class name as a string.
	 */
	static FString GetCarSpawnControllerClassString(ECarSpawnControllerClasses className);

	// Instance methods
public:
	/**
	 * Retrieves the full path of the level to load.
	 *
	 * @return The full level path as a string.
	 */
	FORCEINLINE FString GetLevelPath()
	{
		return LevelDirPath + RelativeLevelPath + "." + RelativeLevelPath;
	}

	/**
	 * Saves the current simulation configuration to a file.
	 */
	UFUNCTION(BlueprintCallable)
	void SaveConfig();

	/**
	 * Loads the simulation configuration from a file.
	 *
	 * @param filename The name of the configuration file to load.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadConfig(FString filename);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CarSpawnController.h"
#include "SimConfig.generated.h"

UENUM()
enum class ECarSpawnControllerClasses
{
	Random,
	Periodic
};

/**
 *
 */
UCLASS()
class TSTOOLKIT_API USimConfig : public UObject
{
	GENERATED_BODY()

private:
	static const FString _MainMenuLevelName;

public:

	USimConfig();

	// File details
	static const FString ConfigFileName;
	static const FString ConfigDirPath;

	// Level details
	static const FString LevelDirPath;

	UPROPERTY(EditAnywhere, Category = "Level Details")
	FString RelativeLevelPath;

	// Simulation global details
	UPROPERTY(EditAnywhere, Category = "Simulation Details")
	float SimulationDuration;

	// Car spawn details
	UPROPERTY(EditAnywhere, Category = "Car Spawning Details")
	ECarSpawnControllerClasses ControllerClassName;

	UPROPERTY(EditAnywhere, Category = "Car Spawning Details")
	float CarsSpawnRate;

	// Screenshot details
	UPROPERTY(EditAnywhere, Category = "Screenshot Details")
	float ScreenshotInterval;

	UPROPERTY(EditAnywhere, Category = "Screenshot Details")
	float DelayBetweenScreenshots;

	// Weather details
	UPROPERTY(EditAnywhere, Category = "Weather Details")
	bool bIsNight;

	UPROPERTY(EditAnywhere, Category = "Weather Details")
	bool bIsOvercast;

	UPROPERTY(EditAnywhere, Category = "Weather Details")
	bool bIsRain;

	// Weather change details
	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	bool bIsChangeDayTime;

	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	float ChangeDayTimeRate;

	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	bool bIsChangeOvercast;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeOvercastRate;

	UPROPERTY(EditAnywhere, Category = "Weather Change Details")
	bool bIsChangeRain;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeRainRate;

	// Static methods
public:
	static TArray<FString> GetLevelNames();
	static TArray<FString> GetCarSpawnControllerClassesNames();
	static ECarSpawnControllerClasses GetCarSpawnControllerClassByName(FString name);
	static FString GetCarSpawnControllerClassString(ECarSpawnControllerClasses className);

	// Instance methods
public:
	FORCEINLINE FString GetLevelPath()
	{
		return LevelDirPath + RelativeLevelPath + "." + RelativeLevelPath;
	}

	UFUNCTION(BlueprintCallable)
	void SaveConfig();

	UFUNCTION(BlueprintCallable)
	void LoadConfig(FString filename);
};
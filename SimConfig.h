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
	static const FString LevelDirrPath;

	UPROPERTY(EditAnywhere, Category = "Level details")
	FString RelativeLevelPath;

	// Simulation global details
	UPROPERTY(EditAnywhere, Category = "Simulation details")
	float SimulationDuration;

	// Car spawn details
	UPROPERTY(EditAnywhere, Category = "Car spawning details")
	ECarSpawnControllerClasses ControllerClassName;

	UPROPERTY(EditAnywhere, Category = "Car spawning details")
	float CarsSpawnRate;

	// Screenshot details
	UPROPERTY(EditAnywhere, Category = "Screenshot details")
	float ScreenshotInterval;

	UPROPERTY(EditAnywhere, Category = "Screenshot details")
	float DelayBetweenScreenshots;

	UPROPERTY(EditAnywhere, Category = "Weather details")
	bool IsNight;

	UPROPERTY(EditAnywhere, Category = "Weather details")
	bool IsOvercast;

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
		return LevelDirrPath + RelativeLevelPath+ "." + RelativeLevelPath;
	}

	UFUNCTION(BlueprintCallable)
	void SaveConfig();

	UFUNCTION(BlueprintCallable)
	void LoadConfig(FString filename);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimConfig.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

private:
	USimConfig* _simConfig;
	
public:


	// Event handlers
	UFUNCTION(BlueprintCallable)
	void StartButtonClick();


	// Getters
	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetRelativeLevelPath()
	{
		return _simConfig->RelativeLevelPath;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSimulationDuration()
	{
		return _simConfig->SimulationDuration;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECarSpawnControllerClasses GetCarSpawnControllerClassName()
	{
		return _simConfig->ControllerClassName;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetCarSpawnControllerClassNameAsString()
	{
		return USimConfig::GetCarSpawnControllerClassString(_simConfig->ControllerClassName);
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCarsSpawnRate()
	{
		return _simConfig->CarsSpawnRate;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetScreenshotInterval()
	{
		return _simConfig->ScreenshotInterval;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDelayBetweenScreenshots()
	{
		return _simConfig->DelayBetweenScreenshots;
	}


	// Setters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetRelativeLevelPath(FString value)
	{
		_simConfig->RelativeLevelPath = value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSimulationDuration(float value)
	{
		_simConfig->SimulationDuration = value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarSpawnControllerClassName(ECarSpawnControllerClasses value)
	{
		_simConfig->ControllerClassName = value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarSpawnControllerClassNameByString(FString value)
	{
		_simConfig->ControllerClassName = USimConfig::GetCarSpawnControllerClassByName(value);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarsSpawnRate(float value)
	{
		_simConfig->CarsSpawnRate = value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetScreenshotInterval(float value)
	{
		_simConfig->ScreenshotInterval = value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDelayBetweenScreenshots(float value)
	{
		_simConfig->DelayBetweenScreenshots = value;
	}

	// Aditional functions
	UFUNCTION(BlueprintPure)
	TArray<FString> GetLevels();

	UFUNCTION(BlueprintPure)
	TArray<FString> GetCarSpawnControllerClasses();
};

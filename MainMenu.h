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
	USimConfig* _SimConfig;

public:

	// Event handlers
	UFUNCTION(BlueprintCallable)
	void StartButtonClick();

	// Getters
	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetRelativeLevelPath() const
	{
		return _SimConfig->RelativeLevelPath;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSimulationDuration() const
	{
		return _SimConfig->SimulationDuration;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECarSpawnControllerClasses GetCarSpawnControllerClassName() const
	{
		return _SimConfig->ControllerClassName;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetCarSpawnControllerClassNameAsString() const
	{
		return USimConfig::GetCarSpawnControllerClassString(_SimConfig->ControllerClassName);
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCarsSpawnRate() const
	{
		return _SimConfig->CarsSpawnRate;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetScreenshotInterval() const
	{
		return _SimConfig->ScreenshotInterval;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDelayBetweenScreenshots() const
	{
		return _SimConfig->DelayBetweenScreenshots;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsNight() const
	{
		return _SimConfig->bIsNight;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsOvercast() const
	{
		return _SimConfig->bIsOvercast;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsRain() const
	{
		return _SimConfig->bIsRain;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsChangeDayTime() const
	{
		return _SimConfig->bIsChangeDayTime;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetChangeDayTimeRate() const
	{
		return _SimConfig->ChangeDayTimeRate;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsChangeOvercast() const
	{
		return _SimConfig->bIsChangeOvercast;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetChangeOvercastRate() const
	{
		return _SimConfig->ChangeOvercastRate;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsChangeRain() const
	{
		return _SimConfig->bIsChangeRain;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetChangeRainRate() const
	{
		return _SimConfig->ChangeRainRate;
	}

	// Setters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetRelativeLevelPath(const FString& Value)
	{
		_SimConfig->RelativeLevelPath = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSimulationDuration(float Value)
	{
		_SimConfig->SimulationDuration = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarSpawnControllerClassName(ECarSpawnControllerClasses Value)
	{
		_SimConfig->ControllerClassName = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarSpawnControllerClassNameByString(const FString& Value)
	{
		_SimConfig->ControllerClassName = USimConfig::GetCarSpawnControllerClassByName(Value);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCarsSpawnRate(float Value)
	{
		_SimConfig->CarsSpawnRate = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetScreenshotInterval(float Value)
	{
		_SimConfig->ScreenshotInterval = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDelayBetweenScreenshots(float Value)
	{
		_SimConfig->DelayBetweenScreenshots = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsNight(bool Value)
	{
		_SimConfig->bIsNight = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsOvercast(bool Value)
	{
		_SimConfig->bIsOvercast = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsRain(bool Value)
	{
		_SimConfig->bIsRain = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsChangeDayTime(bool Value)
	{
		_SimConfig->bIsChangeDayTime = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetChangeDayTimeRate(float Value)
	{
		_SimConfig->ChangeDayTimeRate = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsChangeOvercast(bool Value)
	{
		_SimConfig->bIsChangeOvercast = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetChangeOvercastRate(float Value)
	{
		_SimConfig->ChangeOvercastRate = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsChangeRain(bool Value)
	{
		_SimConfig->bIsChangeRain = Value;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetChangeRainRate(float Value)
	{
		_SimConfig->ChangeRainRate = Value;
	}

	// Additional functions
	UFUNCTION(BlueprintPure)
	TArray<FString> GetLevels() const;

	UFUNCTION(BlueprintPure)
	TArray<FString> GetCarSpawnControllerClasses() const;

	UFUNCTION(BlueprintCallable)
	void SaveConfig();
};
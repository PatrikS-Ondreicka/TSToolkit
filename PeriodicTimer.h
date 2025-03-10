// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PeriodicTimer.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API UPeriodicTimer : public UObject
{
	GENERATED_BODY()

public:
	UPeriodicTimer();

protected:
	float _initValue;
	float _currentCountdown;

public:

	FORCEINLINE float GetInitValue() const
	{
		return _initValue;
	}

	FORCEINLINE void SetInitValue(float value)
	{
		_initValue = value;
		ResetCountdown();
	}

	FORCEINLINE float GetCurrentCountdown() const
	{
		return _currentCountdown;
	}

	FORCEINLINE bool CoundownState() const
	{
		return _currentCountdown <= 0.0f;
	}

	FORCEINLINE void DecrementCountdown(float value)
	{
		_currentCountdown -= value;
	}

	FORCEINLINE void ResetCountdown()
	{
		_currentCountdown = _initValue;
	}
};


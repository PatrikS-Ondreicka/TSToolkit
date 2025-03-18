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
	float _InitValue;
	float _CurrentCountdown;

public:

	FORCEINLINE float GetInitValue() const
	{
		return _InitValue;
	}

	FORCEINLINE void SetInitValue(float Value)
	{
		_InitValue = Value;
		ResetCountdown();
	}

	FORCEINLINE float GetCurrentCountdown() const
	{
		return _CurrentCountdown;
	}

	FORCEINLINE bool CountdownState() const
	{
		return _CurrentCountdown <= 0.0f;
	}

	FORCEINLINE void DecrementCountdown(float Value)
	{
		_CurrentCountdown -= Value;
	}

	FORCEINLINE void ResetCountdown()
	{
		_CurrentCountdown = _InitValue;
	}
};
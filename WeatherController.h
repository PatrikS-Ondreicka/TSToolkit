// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeriodicTimer.h"
#include "WeatherController.generated.h"

UENUM()
enum class EDayTimeTypes
{
	Day,
	Night
};

EDayTimeTypes GetNextDaytimeType(EDayTimeTypes type);

UENUM()
enum class EOvercastTypes
{
	Overcast,
	Clear
};

EOvercastTypes GetNextOvercastType(EOvercastTypes type);

UENUM()
enum class ERainTypes
{
	NoRain,
	Rain
};

ERainTypes GetNextRainType(ERainTypes type);

UCLASS()
class TSTOOLKIT_API AWeatherController : public AActor
{
	GENERATED_BODY()

public:
	AWeatherController();

	// Components
	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class UDirectionalLightComponent* Sun;

	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class UVolumetricCloudComponent* VolumetricCloud;

	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class UNiagaraComponent* RainComponent;

	// Weather settings
	UPROPERTY(EditAnywhere, Category = "Weather Settings")
	EDayTimeTypes CurrentDayTime = EDayTimeTypes::Day;

	UPROPERTY(EditAnywhere, Category = "Weather Settings")
	EOvercastTypes CurrentOvercast = EOvercastTypes::Clear;

	UPROPERTY(EditAnywhere, Category = "Weather Settings")
	ERainTypes CurrentRain = ERainTypes::NoRain;

	// Weather change settings
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	bool ChangeDayTime = false;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeDayTimeRate = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	bool ChangeOvercast = false;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeOvercastRate = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	bool ChangeRain = false;

	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeRainRate = 60.0f;

	// Read only sun default values
	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float DaySunIntensity = 30000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float NightSunIntensity = 1000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float DayOvercastIntensity = 20000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float NightOvercastIntensity = 500.0f;

	// Read only sky default values
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float ClearMieScattering = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float OvercastMieScattering = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float ClearMieAnisotropy = 0.8f;

	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float OvercastMieAnisotropy = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float ClearRayleighScattering = 0.05f;

	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float OvercastRayleighScattering = 0.0f;

protected:
	UPeriodicTimer* _DaytimeChangeTimer;
	UPeriodicTimer* _OvercastChangeTimer;
	UPeriodicTimer* _RainChangeTimer;

private:
	void _SetDay(EOvercastTypes overcast);
	void _SetNight(EOvercastTypes overcast);
	void _SetCloudOvercast(EOvercastTypes overcast);
	void _TurnOnLamps();
	void _TurnOffLamps();
	void _SetNightForControllers(bool state);
	void _InitVolumetricCloud();
	void _SetRain();
	void _SetNoRain();
	bool _HandleTimer(UPeriodicTimer* timer, float deltaTime);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float deltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void SetWeather(EDayTimeTypes time, EOvercastTypes overcast);

	UFUNCTION(BlueprintCallable)
	void SetRain(ERainTypes rain);
};


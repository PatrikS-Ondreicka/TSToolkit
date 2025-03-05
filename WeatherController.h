// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherController.generated.h"

UENUM()
enum class EDayTimeTypes
{
	Day,
	Night
};

UENUM()
enum class EOvercastTypes
{
	Overcast,
	Clear
};

UCLASS()
class TSTOOLKIT_API AWeatherController : public AActor
{
	GENERATED_BODY()

public:
	AWeatherController();

	// Components
	UPROPERTY(EditAnywhere, Category = "Weather components")
	class UDirectionalLightComponent* Sun;

	UPROPERTY(EditAnywhere, Category = "Weather components")
	class UVolumetricCloudComponent* VolumetricCloud;

	UPROPERTY(EditAnywhere, Category = "Weather components")
	class USkyAtmosphereComponent* SkyAtmosphere;

	// Weather settings
	UPROPERTY(EditAnywhere, Category = "Weather settings")
	EDayTimeTypes CurrentDayTime = EDayTimeTypes::Day;

	UPROPERTY(EditAnywhere, Category = "Weather settings")
	EOvercastTypes CurrentOvercast = EOvercastTypes::Clear;
	
	// Read only sun default values
	UPROPERTY(VisibleAnywhere, Category = "Sun defaul values")
	float DaySunIntensity = 30000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sun defaul values")
	float NightSunIntensity = 1000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sun defaul values")
	float DayOvercastIntensity = 20000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sun defaul values")
	float NightOvercastIntensity = 500.0f;

	// Read only sky default values
	UPROPERTY(VisibleAnywhere, Category = "Sky default values")
	float ClearMieScattering = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sky default values")
	float OvercastMieScattering = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = "Sky default values")
	float ClearMieAnisotropy = 0.8f;

	UPROPERTY(VisibleAnywhere, Category = "Sky default values")
	float OvercastMieAnisotropy = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sky default values")
	float ClearRayleighScattering = 0.05f;

	UPROPERTY(VisibleAnywhere, Category = "Sky default values")
	float OvercastRayleighScattering = 0.0f;


private:
	void _setDay(EOvercastTypes overcast);
	void _setNight(EOvercastTypes overcast);
	void _setCloudOvercast(EOvercastTypes overcast);
	void _turnOnLamps();
	void _turnOffLamps();
	void _setNightForControllers(bool state);
	void _initVolumetricCloud();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeather(EDayTimeTypes time, EOvercastTypes overcast);

};

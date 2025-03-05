// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherController.h"
#include "Components/SkylightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Lamp.h"
#include "CarSpawnController.h"

typedef UGameplayStatics GS;

// Sets default values
AWeatherController::AWeatherController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	RootComponent = Sun;

	VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));
	VolumetricCloud->SetupAttachment(Sun);

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(Sun);

	_initVolumetricCloud();
}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	SetWeather(CurrentDayTime, CurrentOvercast);
}

// Called every frame
void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeatherController::SetWeather(EDayTimeTypes time, EOvercastTypes overcast)
{
	if (time == EDayTimeTypes::Day)
	{
		_setDay(overcast);
	}
	else if (time == EDayTimeTypes::Night)
	{
		_setNight(overcast);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid day time"));
	}
}

void AWeatherController::_setDay(EOvercastTypes overcast)
{
	float sunIntentisy = (overcast == EOvercastTypes::Clear) ? DaySunIntensity : DayOvercastIntensity;
	Sun->SetIntensity(sunIntentisy);
	Sun->SetLightSourceAngle(2.0f);
	FRotator newRotation = FRotator(270.0f, -80.0f, 270.0f);
	Sun->SetWorldRotation(newRotation);
	_setNightForControllers(false);
	_setCloudOvercast(overcast);
	_turnOffLamps();
}

void AWeatherController::_setNight(EOvercastTypes overcast)
{
	float sunIntentisy = (overcast == EOvercastTypes::Clear) ? NightSunIntensity : NightOvercastIntensity;
	Sun->SetIntensity(sunIntentisy);
	Sun->SetLightSourceAngle(2.0f);
	FRotator newRotation = FRotator(270.0f, -80.0f, 270.0f);
	Sun->SetWorldRotation(newRotation);
	_setNightForControllers(true);
	_setCloudOvercast(overcast);
	_turnOnLamps();
}

void AWeatherController::_setCloudOvercast(EOvercastTypes overcast)
{
	if (overcast == EOvercastTypes::Clear)
	{
		SkyAtmosphere->SetMieScatteringScale(ClearMieScattering);
		SkyAtmosphere->SetMieAnisotropy(ClearMieAnisotropy);
		SkyAtmosphere->SetRayleighScatteringScale(ClearRayleighScattering);
		VolumetricCloud->SetVisibility(false);
	}
	else if (overcast == EOvercastTypes::Overcast)
	{
		SkyAtmosphere->SetMieScatteringScale(OvercastMieScattering);
		SkyAtmosphere->SetMieAnisotropy(OvercastMieAnisotropy);
		SkyAtmosphere->SetRayleighScatteringScale(OvercastRayleighScattering);
		VolumetricCloud->SetVisibility(true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid overcast"));
	}
}

void AWeatherController::_turnOnLamps()
{
	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), ALamp::StaticClass(), found);
	for (AActor* actor : found)
	{
		ALamp* lamp = Cast<ALamp>(actor);
		if (lamp && lamp->TurnOnWhenNight)
		{
			lamp->TurnOn();
		}
	}
}

void AWeatherController::_turnOffLamps()
{
	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), ALamp::StaticClass(), found);
	for (AActor* actor : found)
	{
		ALamp* lamp = Cast<ALamp>(actor);
		if (lamp)
		{
			lamp->TurnOff();
		}
	}
}

void AWeatherController::_setNightForControllers(bool state)
{
	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), ACarSpawnController::StaticClass(), found);
	for (AActor* actor : found)
	{
		ACarSpawnController* controller = Cast<ACarSpawnController>(actor);
		if (controller)
		{
			controller->SetNight(state);
		}
	}
}

void AWeatherController::_initVolumetricCloud()
{
	if (VolumetricCloud)
	{
		VolumetricCloud->SetLayerHeight(2.0f);
		VolumetricCloud->SetLayerBottomAltitude(2.0f);
		VolumetricCloud->SetTracingMaxDistance(50.0f);
	}
}


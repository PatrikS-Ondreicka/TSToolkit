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
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

typedef UGameplayStatics GS;

// Sets default values
AWeatherController::AWeatherController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	RootComponent = Sun;

	VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));
	VolumetricCloud->SetupAttachment(RootComponent);

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(RootComponent);

	RainComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Rain component"));
	RainComponent->bAutoActivate = true;
	_InitVolumetricCloud();

	// Timer settings
	_DaytimeChangeTimer = NewObject<UPeriodicTimer>();
	_OvercastChangeTimer = NewObject<UPeriodicTimer>();
	_RainChangeTimer = NewObject<UPeriodicTimer>();
}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	SetWeather(CurrentDayTime, CurrentOvercast);
	SetRain(CurrentRain);
	RainComponent->ActivateSystem();
	_DaytimeChangeTimer->SetInitValue(ChangeDayTimeRate);
	_OvercastChangeTimer->SetInitValue(ChangeOvercastRate);
	_RainChangeTimer->SetInitValue(ChangeRainRate);
}

// Called every frame
void AWeatherController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (!(ChangeDayTime || ChangeOvercast || ChangeRain))
	{
		return;
	}
	EDayTimeTypes dayTime = CurrentDayTime;
	EOvercastTypes overcast = CurrentOvercast;
	ERainTypes rain = CurrentRain;

	bool weatherChange = false;

	if (ChangeDayTime)
	{
		if (_HandleTimer(_DaytimeChangeTimer, deltaTime))
		{
			dayTime = GetNextDaytimeType(CurrentDayTime);
			weatherChange = true;
		}
	}

	if (ChangeOvercast)
	{
		if (_HandleTimer(_OvercastChangeTimer, deltaTime))
		{
			overcast = GetNextOvercastType(CurrentOvercast);
			weatherChange = true;
		}
	}

	if (ChangeRain)
	{
		if (_HandleTimer(_RainChangeTimer, deltaTime))
		{
			rain = GetNextRainType(CurrentRain);
			weatherChange = true;
		}
	}

	if (weatherChange)
	{
		SetWeather(dayTime, overcast);
		SetRain(rain);
	}
}

void AWeatherController::OnConstruction(const FTransform& Transform)
{
	SetWeather(CurrentDayTime, CurrentOvercast);
	SetRain(CurrentRain);
}

void AWeatherController::SetWeather(EDayTimeTypes time, EOvercastTypes overcast)
{
	if (time == EDayTimeTypes::Day)
	{
		_SetDay(overcast);
	}
	else if (time == EDayTimeTypes::Night)
	{
		_SetNight(overcast);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid day time"));
	}
}

void AWeatherController::SetRain(ERainTypes rain)
{
	if (rain == ERainTypes::NoRain)
	{
		_SetNoRain();
	}
	else if (rain == ERainTypes::Rain)
	{
		_SetRain();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid rain type"));
	}
}

void AWeatherController::_SetDay(EOvercastTypes overcast)
{
	float sunIntensity = (overcast == EOvercastTypes::Clear) ? DaySunIntensity : DayOvercastIntensity;
	Sun->SetIntensity(sunIntensity);
	Sun->SetLightSourceAngle(2.0f);
	FRotator newRotation = FRotator(270.0f, -80.0f, 270.0f);
	Sun->SetWorldRotation(newRotation);
	_SetNightForControllers(false);
	_SetCloudOvercast(overcast);
	_TurnOffLamps();
	CurrentDayTime = EDayTimeTypes::Day;
}

void AWeatherController::_SetNight(EOvercastTypes overcast)
{
	float sunIntensity = (overcast == EOvercastTypes::Clear) ? NightSunIntensity : NightOvercastIntensity;
	Sun->SetIntensity(sunIntensity);
	Sun->SetLightSourceAngle(2.0f);
	FRotator newRotation = FRotator(270.0f, -80.0f, 270.0f);
	Sun->SetWorldRotation(newRotation);
	_SetNightForControllers(true);
	_SetCloudOvercast(overcast);
	_TurnOnLamps();
	CurrentDayTime = EDayTimeTypes::Night;
}

void AWeatherController::_SetCloudOvercast(EOvercastTypes overcast)
{
	if (overcast == EOvercastTypes::Clear)
	{
		SkyAtmosphere->SetMieScatteringScale(ClearMieScattering);
		SkyAtmosphere->SetMieAnisotropy(ClearMieAnisotropy);
		SkyAtmosphere->SetRayleighScatteringScale(ClearRayleighScattering);
		VolumetricCloud->SetVisibility(false);
		CurrentOvercast = EOvercastTypes::Clear;
	}
	else if (overcast == EOvercastTypes::Overcast)
	{
		SkyAtmosphere->SetMieScatteringScale(OvercastMieScattering);
		SkyAtmosphere->SetMieAnisotropy(OvercastMieAnisotropy);
		SkyAtmosphere->SetRayleighScatteringScale(OvercastRayleighScattering);
		VolumetricCloud->SetVisibility(true);
		CurrentOvercast = EOvercastTypes::Overcast;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid overcast"));
	}
}

void AWeatherController::_TurnOnLamps()
{
	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), ALamp::StaticClass(), found);
	for (AActor* actor : found)
	{
		ALamp* lamp = Cast<ALamp>(actor);
		if (lamp && lamp->bTurnOnWhenNight)
		{
			lamp->TurnOn();
		}
	}
}

void AWeatherController::_TurnOffLamps()
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

void AWeatherController::_SetNightForControllers(bool state)
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

void AWeatherController::_InitVolumetricCloud()
{
	if (VolumetricCloud)
	{
		VolumetricCloud->SetLayerHeight(2.0f);
		VolumetricCloud->SetLayerBottomAltitude(2.0f);
		VolumetricCloud->SetTracingMaxDistance(50.0f);
	}
}

void AWeatherController::_SetRain()
{
	if (RainComponent)
	{
		RainComponent->SetVisibility(true);
	}
	CurrentRain = ERainTypes::Rain;
}

void AWeatherController::_SetNoRain()
{
	if (RainComponent)
	{
		RainComponent->SetVisibility(false);
	}
	CurrentRain = ERainTypes::NoRain;
}

bool AWeatherController::_HandleTimer(UPeriodicTimer* timer, float deltaTime)
{
	timer->DecrementCountdown(deltaTime);
	bool currentState = timer->CountdownState();
	if (currentState)
	{
		timer->ResetCountdown();
	}
	return currentState;
}

// Enum helper functions

EDayTimeTypes GetNextDaytimeType(EDayTimeTypes type)
{
	if (type == EDayTimeTypes::Day)
	{
		return EDayTimeTypes::Night;
	}
	else if (type == EDayTimeTypes::Night)
	{
		return EDayTimeTypes::Day;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid type"));
		return type;
	}
}

EOvercastTypes GetNextOvercastType(EOvercastTypes type)
{
	if (type == EOvercastTypes::Clear)
	{
		return EOvercastTypes::Overcast;
	}
	else if (type == EOvercastTypes::Overcast)
	{
		return EOvercastTypes::Clear;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid type"));
		return type;
	}
}

ERainTypes GetNextRainType(ERainTypes type)
{
	if (type == ERainTypes::NoRain)
	{
		return ERainTypes::Rain;
	}
	else if (type == ERainTypes::Rain)
	{
		return ERainTypes::NoRain;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid type"));
		return type;
	}
}

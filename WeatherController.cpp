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
	_initVolumetricCloud();

	// Timer settings
	_daytimeChangeTimer = NewObject<UPeriodicTimer>();
	_overcastChangeTimer = NewObject<UPeriodicTimer>();
	_rainChangeTimer = NewObject<UPeriodicTimer>();
}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	SetWeather(CurrentDayTime, CurrentOvercast);
	SetRain(CurrentRain);
	RainComponent->ActivateSystem();
	_daytimeChangeTimer->SetInitValue(ChangeDayTimeRate);
	_overcastChangeTimer->SetInitValue(ChangeOvercastRate);
	_rainChangeTimer->SetInitValue(ChangeRainRate);
}

// Called every frame
void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		if (_handleTimer(_daytimeChangeTimer, DeltaTime))
		{
			dayTime = GetNextDaytimeType(CurrentDayTime);
			weatherChange = true;
		}
	}

	if (ChangeOvercast)
	{
		if (_handleTimer(_overcastChangeTimer, DeltaTime))
		{
			overcast = GetNextOvercastType(CurrentOvercast);
			weatherChange = true;
		}
	}

	if (ChangeRain)
	{
		if (_handleTimer(_rainChangeTimer, DeltaTime))
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

void AWeatherController::SetRain(ERainTypes rain)
{
	if (rain == ERainTypes::NoRain)
	{
		_setNoRain();
	}
	else if (rain == ERainTypes::Rain)
	{
		_setRain();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid rain type"));
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
	CurrentDayTime = EDayTimeTypes::Day;
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
	CurrentDayTime = EDayTimeTypes::Night;
}

void AWeatherController::_setCloudOvercast(EOvercastTypes overcast)
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

void AWeatherController::_setRain()
{
	if (RainComponent)
	{
		RainComponent->SetVisibility(true);
	}
	CurrentRain = ERainTypes::Rain;
}

void AWeatherController::_setNoRain()
{
	if (RainComponent)
	{
		RainComponent->SetVisibility(false);
	}
	CurrentRain = ERainTypes::NoRain;
}

bool AWeatherController::_handleTimer(UPeriodicTimer* timer, float DeltaTime)
{
	timer->DecrementCountdown(DeltaTime);
	bool currentState = timer->CoundownState();
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

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
}

void AWeatherController::_ChangeDayTimeAction()
{
	auto nextState = GetNextDaytimeType(CurrentDayTime);
	SetWeather(nextState, CurrentOvercast);
	_ResetTimer(ChangeDayTimeRate, &AWeatherController::_ChangeDayTimeAction);
}

void AWeatherController::_ChangeOvercastAction()
{
	auto nextState = GetNextOvercastType(CurrentOvercast);
	SetWeather(CurrentDayTime, nextState);
	_ResetTimer(ChangeOvercastRate, &AWeatherController::_ChangeOvercastAction);
}

void AWeatherController::_ChangeRainAction()
{
	auto nextState = GetNextRainType(CurrentRain);
	SetRain(nextState);
	_ResetTimer(ChangeRainRate, &AWeatherController::_ChangeRainAction);
}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	SetWeather(CurrentDayTime, CurrentOvercast);
	SetRain(CurrentRain);
	RainComponent->ActivateSystem();
	SetUpTimers();
}

// Called every frame
void AWeatherController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
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

void AWeatherController::SetUpTimers()
{
	if (ChangeDayTime)
	{
		_ResetTimer(ChangeDayTimeRate, &AWeatherController::_ChangeDayTimeAction);
	}

	if (ChangeOvercast)
	{
		_ResetTimer(ChangeOvercastRate, &AWeatherController::_ChangeOvercastAction);
	}

	if (ChangeRain)
	{
		_ResetTimer(ChangeRainRate, &AWeatherController::_ChangeRainAction);
	}
}

void AWeatherController::_ResetTimer(float Rate, void(AWeatherController::* InTimerFunction)())
{
	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, InTimerFunction, ChangeDayTimeRate, false);
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

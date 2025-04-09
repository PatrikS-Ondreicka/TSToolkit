// Fill out your copyright notice in the Description page of Project Settings.

#include "WeatherController.h"
#include "Components/SkylightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Lamp.h"
#include "Puddle.h"
#include "CarSpawnController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

typedef UGameplayStatics GS;

/**
 * Constructor for AWeatherController.
 * Initializes default values for weather components and settings.
 */
AWeatherController::AWeatherController()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	if (!Sun)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Sun component in AWeatherController constructor."));
	}
	RootComponent = Sun;

	VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));
	if (!VolumetricCloud)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create VolumetricCloud component in AWeatherController constructor."));
	}
	VolumetricCloud->SetupAttachment(RootComponent);

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	if (!SkyAtmosphere)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SkyAtmosphere component in AWeatherController constructor."));
	}
	SkyAtmosphere->SetupAttachment(RootComponent);

	RainComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RainComponent"));
	if (!RainComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create RainComponent in AWeatherController constructor."));
	}
	RainComponent->bAutoActivate = true;

	_InitVolumetricCloud();
}

/**
 * Changes the day/night cycle to the next state and resets the timer.
 */
void AWeatherController::_ChangeDayTimeAction()
{
	auto nextState = GetNextDaytimeType(CurrentDayTime);
	SetWeather(nextState, CurrentOvercast);
	_ResetTimer(ChangeDayTimeRate, &AWeatherController::_ChangeDayTimeAction);
}

/**
 * Changes the overcast state to the next state and resets the timer.
 */
void AWeatherController::_ChangeOvercastAction()
{
	auto nextState = GetNextOvercastType(CurrentOvercast);
	SetWeather(CurrentDayTime, nextState);
	_ResetTimer(ChangeOvercastRate, &AWeatherController::_ChangeOvercastAction);
}

/**
 * Changes the rain state to the next state and resets the timer.
 */
void AWeatherController::_ChangeRainAction()
{
	auto nextState = GetNextRainType(CurrentRain);
	SetRain(nextState);
	_ResetTimer(ChangeRainRate, &AWeatherController::_ChangeRainAction);
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes weather and rain settings and sets up timers.
 */
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();

	SetWeather(CurrentDayTime, CurrentOvercast);
	SetRain(CurrentRain);

	if (RainComponent)
	{
		RainComponent->ActivateSystem();
		RainComponent->bAutoActivate = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RainComponent is null in BeginPlay."));
	}

	SetUpTimers();
}

/**
 * Performs an action on all actors of a specified class.
 *
 * @param ActorClass The class of actors to target.
 * @param Action The action to perform on each actor.
 */
void AWeatherController::_PerformActionOnAllActors(TSubclassOf<AActor> ActorClass, void(*Action)(AActor*))
{
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("_PerformActionOnAllActors: ActorClass is null."));
		return;
	}

	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), ActorClass, found);

	for (AActor* actor : found)
	{
		if (actor)
		{
			Action(actor);
		}
	}
}

/**
 * Sets a state on all actors of a specified class.
 *
 * @param ActorClass The class of actors to target.
 * @param state The state to set.
 * @param Action The action to perform on each actor with the state.
 */
void AWeatherController::_SetStateOnAllActors(TSubclassOf<AActor> ActorClass, bool state, void(*Action)(AActor*, bool))
{
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetStateOnAllActors: ActorClass is null."));
		return;
	}

	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), ActorClass, found);

	for (AActor* actor : found)
	{
		if (actor)
		{
			Action(actor, state);
		}
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param deltaTime The time elapsed since the last frame.
 */
void AWeatherController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

/**
 * Called when the actor is constructed or properties are changed in the editor.
 * Updates weather and rain settings.
 *
 * @param Transform The transform of the actor.
 */
void AWeatherController::OnConstruction(const FTransform& Transform)
{
	SetWeather(CurrentDayTime, CurrentOvercast);
	SetRain(CurrentRain);
}

/**
 * Sets the weather to the specified day time and overcast type.
 *
 * @param time The day time type to set.
 * @param overcast The overcast type to set.
 */
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid day time in SetWeather."));
	}
}

/**
 * Sets the rain state in the simulation.
 *
 * @param rain The rain type to set.
 */
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid rain type in SetRain."));
	}
}

/**
 * Sets the weather to daytime with the specified overcast type.
 *
 * @param overcast The overcast type to set.
 */
void AWeatherController::_SetDay(EOvercastTypes overcast)
{
	if (!Sun)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetDay: Sun component is null."));
		return;
	}

	float sunIntensity = (overcast == EOvercastTypes::Clear) ? DaySunIntensity : DayOvercastIntensity;
	Sun->SetIntensity(sunIntensity);
	Sun->SetLightSourceAngle(0.54f);
	FRotator newRotation = FRotator(270.0f, -80.0f, 270.0f);
	Sun->SetWorldRotation(newRotation);

	_SetNightForControllers(false);
	_SetCloudOvercast(overcast);
	_TurnOffLamps();
	CurrentDayTime = EDayTimeTypes::Day;
}

/**
 * Sets the weather to nighttime with the specified overcast type.
 *
 * @param overcast The overcast type to set.
 */
void AWeatherController::_SetNight(EOvercastTypes overcast)
{
	if (!Sun)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetNight: Sun component is null."));
		return;
	}

	float sunIntensity = (overcast == EOvercastTypes::Clear) ? NightSunIntensity : NightOvercastIntensity;
	Sun->SetIntensity(sunIntensity);
	Sun->SetLightSourceAngle(0.54f);
	FRotator newRotation = FRotator(270.0f, -80.0f, 270.0f);
	Sun->SetWorldRotation(newRotation);

	_SetNightForControllers(true);
	_SetCloudOvercast(overcast);
	_TurnOnLamps();
	CurrentDayTime = EDayTimeTypes::Night;
}

/**
 * Configures the cloud component for the specified overcast type.
 *
 * @param overcast The overcast type to set.
 */
void AWeatherController::_SetCloudOvercast(EOvercastTypes overcast)
{
	if (!SkyAtmosphere || !VolumetricCloud)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetCloudOvercast: SkyAtmosphere or VolumetricCloud is null."));
		return;
	}

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid overcast type in _SetCloudOvercast."));
	}
}

/**
 * Turns on all lamps in the simulation.
 */
void AWeatherController::_TurnOnLamps()
{
	_PerformActionOnAllActors(ALamp::StaticClass(), [](AActor* actor)
		{
			ALamp* lamp = Cast<ALamp>(actor);
			if (lamp)
			{
				lamp->TurnOn();
			}
		});
}

/**
 * Turns off all lamps in the simulation.
 */
void AWeatherController::_TurnOffLamps()
{
	_PerformActionOnAllActors(ALamp::StaticClass(), [](AActor* actor)
		{
			ALamp* lamp = Cast<ALamp>(actor);
			if (lamp)
			{
				lamp->TurnOff();
			}
		});
}

/**
 * Sets the nighttime state for all controllers in the simulation.
 *
 * @param state True to enable nighttime, false otherwise.
 */
void AWeatherController::_SetNightForControllers(bool state)
{
	_SetStateOnAllActors(ACarSpawnController::StaticClass(), state, [](AActor* actor, bool newState)
		{
			ACarSpawnController* controller = Cast<ACarSpawnController>(actor);
			if (controller)
			{
				controller->SetNight(newState);
			}
		});
}

/**
 * Initializes the volumetric cloud component.
 */
void AWeatherController::_InitVolumetricCloud()
{
	if (!VolumetricCloud)
	{
		UE_LOG(LogTemp, Error, TEXT("_InitVolumetricCloud: VolumetricCloud is null."));
		return;
	}

	VolumetricCloud->SetLayerHeight(2.0f);
	VolumetricCloud->SetLayerBottomAltitude(2.0f);
	VolumetricCloud->SetTracingMaxDistance(50.0f);
}

/**
 * Enables rain effects in the simulation.
 */
void AWeatherController::_SetRain()
{
	if (RainComponent)
	{
		RainComponent->SetVisibility(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("_SetRain: RainComponent is null."));
	}

	_SetPuddlesVisiblity(true);
	CurrentRain = ERainTypes::Rain;
}

/**
 * Disables rain effects in the simulation.
 */
void AWeatherController::_SetNoRain()
{
	if (RainComponent)
	{
		RainComponent->SetVisibility(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("_SetNoRain: RainComponent is null."));
	}

	_SetPuddlesVisiblity(false);
	CurrentRain = ERainTypes::NoRain;
}

/**
 * Sets the visibility of puddles in the simulation.
 *
 * @param State True to make puddles visible, false otherwise.
 */
void AWeatherController::_SetPuddlesVisiblity(bool State)
{
	_SetStateOnAllActors(APuddle::StaticClass(), State, [](AActor* actor, bool newState)
		{
			APuddle* puddle = Cast<APuddle>(actor);
			if (puddle)
			{
				puddle->SetActorHiddenInGame(!newState);
			}
		});
}

/**
 * Sets up timers for dynamic weather changes.
 */
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

/**
 * Resets a timer with the specified rate and function.
 *
 * @param Rate The duration of the timer, in seconds.
 * @param InTimerFunction The function to call when the timer expires.
 */
void AWeatherController::_ResetTimer(float Rate, void(AWeatherController::* InTimerFunction)())
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("_ResetTimer: World is null."));
		return;
	}

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, InTimerFunction, Rate, false);
}

// Enum helper functions

/**
 * Retrieves the next day time type in the cycle.
 *
 * @param type The current day time type.
 * @return The next day time type.
 */
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid type in GetNextDaytimeType."));
		return type;
	}
}

/**
 * Retrieves the next overcast type in the cycle.
 *
 * @param type The current overcast type.
 * @return The next overcast type.
 */
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid type in GetNextOvercastType."));
		return type;
	}
}

/**
 * Retrieves the next rain type in the cycle.
 *
 * @param type The current rain type.
 * @return The next rain type.
 */
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid type in GetNextRainType."));
		return type;
	}
}

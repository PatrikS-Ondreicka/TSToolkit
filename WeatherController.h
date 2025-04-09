// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherController.generated.h"

/**
 * Enum representing the types of day time.
 * - Day: Represents daytime.
 * - Night: Represents nighttime.
 */
UENUM()
enum class EDayTimeTypes
{
	Day,
	Night
};

/**
 * Retrieves the next day time type in the cycle.
 *
 * @param type The current day time type.
 * @return The next day time type.
 */
EDayTimeTypes GetNextDaytimeType(EDayTimeTypes type);

/**
 * Enum representing the types of overcast weather.
 * - Overcast: Represents cloudy weather.
 * - Clear: Represents clear weather.
 */
UENUM()
enum class EOvercastTypes
{
	Overcast,
	Clear
};

/**
 * Retrieves the next overcast type in the cycle.
 *
 * @param type The current overcast type.
 * @return The next overcast type.
 */
EOvercastTypes GetNextOvercastType(EOvercastTypes type);

/**
 * Enum representing the types of rain.
 * - NoRain: Represents no rain.
 * - Rain: Represents rainy weather.
 */
UENUM()
enum class ERainTypes
{
	NoRain,
	Rain
};

/**
 * Retrieves the next rain type in the cycle.
 *
 * @param type The current rain type.
 * @return The next rain type.
 */
ERainTypes GetNextRainType(ERainTypes type);

/**
 * AWeatherController is responsible for managing weather conditions in the simulation.
 * It controls day/night cycles, overcast conditions, and rain, and provides functionality
 * to dynamically change these conditions during runtime.
 */
UCLASS()
class TSTOOLKIT_API AWeatherController : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for AWeatherController.
	 * Initializes default values for weather components and settings.
	 */
	AWeatherController();

	// Components

	/** Directional light component representing the sun. */
	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class UDirectionalLightComponent* Sun;

	/** Volumetric cloud component for rendering clouds. */
	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class UVolumetricCloudComponent* VolumetricCloud;

	/** Sky atmosphere component for rendering the sky. */
	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class USkyAtmosphereComponent* SkyAtmosphere;

	/** Niagara component for rendering rain effects. */
	UPROPERTY(EditAnywhere, Category = "Weather Components")
	class UNiagaraComponent* RainComponent;

	// Weather settings

	/** Current day time type (Day or Night). */
	UPROPERTY(EditAnywhere, Category = "Weather Settings")
	EDayTimeTypes CurrentDayTime = EDayTimeTypes::Day;

	/** Current overcast type (Overcast or Clear). */
	UPROPERTY(EditAnywhere, Category = "Weather Settings")
	EOvercastTypes CurrentOvercast = EOvercastTypes::Clear;

	/** Current rain type (Rain or NoRain). */
	UPROPERTY(EditAnywhere, Category = "Weather Settings")
	ERainTypes CurrentRain = ERainTypes::NoRain;

	// Weather change settings

	/** Whether to enable dynamic day/night changes. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	bool ChangeDayTime = false;

	/** Rate of day/night changes, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeDayTimeRate = 60.0f;

	/** Whether to enable dynamic overcast changes. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	bool ChangeOvercast = false;

	/** Rate of overcast changes, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeOvercastRate = 60.0f;

	/** Whether to enable dynamic rain changes. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	bool ChangeRain = false;

	/** Rate of rain changes, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Weather Change Settings")
	float ChangeRainRate = 60.0f;

	// Read-only sun default values

	/** Default sun intensity during the day. */
	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float DaySunIntensity = 30000.0f;

	/** Default sun intensity during the night. */
	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float NightSunIntensity = 1000.0f;

	/** Default sun intensity during overcast day. */
	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float DayOvercastIntensity = 20000.0f;

	/** Default sun intensity during overcast night. */
	UPROPERTY(VisibleAnywhere, Category = "Sun Default Values")
	float NightOvercastIntensity = 500.0f;

	// Read-only sky default values

	/** Default Mie scattering value for clear skies. */
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float ClearMieScattering = 0.0f;

	/** Default Mie scattering value for overcast skies. */
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float OvercastMieScattering = 0.5f;

	/** Default Mie anisotropy value for clear skies. */
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float ClearMieAnisotropy = 0.8f;

	/** Default Mie anisotropy value for overcast skies. */
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float OvercastMieAnisotropy = 0.0f;

	/** Default Rayleigh scattering value for clear skies. */
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float ClearRayleighScattering = 0.05f;

	/** Default Rayleigh scattering value for overcast skies. */
	UPROPERTY(VisibleAnywhere, Category = "Sky Default Values")
	float OvercastRayleighScattering = 0.0f;

private:
	/**
	 * Sets the weather to daytime with the specified overcast type.
	 *
	 * @param overcast The overcast type to set.
	 */
	void _SetDay(EOvercastTypes overcast);

	/**
	 * Sets the weather to nighttime with the specified overcast type.
	 *
	 * @param overcast The overcast type to set.
	 */
	void _SetNight(EOvercastTypes overcast);

	/**
	 * Configures the cloud component for the specified overcast type.
	 *
	 * @param overcast The overcast type to set.
	 */
	void _SetCloudOvercast(EOvercastTypes overcast);

	/**
	 * Turns on all lamps in the simulation.
	 */
	void _TurnOnLamps();

	/**
	 * Turns off all lamps in the simulation.
	 */
	void _TurnOffLamps();

	/**
	 * Sets the nighttime state for all controllers in the simulation.
	 *
	 * @param state True to enable nighttime, false otherwise.
	 */
	void _SetNightForControllers(bool state);

	/**
	 * Initializes the volumetric cloud component.
	 */
	void _InitVolumetricCloud();

	/**
	 * Enables rain effects in the simulation.
	 */
	void _SetRain();

	/**
	 * Disables rain effects in the simulation.
	 */
	void _SetNoRain();

	/**
	 * Sets the visibility of puddles in the simulation.
	 *
	 * @param State True to make puddles visible, false otherwise.
	 */
	void _SetPuddlesVisiblity(bool State);

	// Timer methods

	/**
	 * Resets a timer with the specified rate and function.
	 *
	 * @param Rate The duration of the timer, in seconds.
	 * @param InTimerFunction The function to call when the timer expires.
	 */
	void _ResetTimer(float Rate, void(AWeatherController::* InTimerFunction)());

	// Timer action methods

	/**
	 * Action to perform when changing the day/night cycle.
	 */
	void _ChangeDayTimeAction();

	/**
	 * Action to perform when changing the overcast state.
	 */
	void _ChangeOvercastAction();

	/**
	 * Action to perform when changing the rain state.
	 */
	void _ChangeRainAction();

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic specific to the weather controller.
	 */
	virtual void BeginPlay() override;

	/**
	 * Performs an action on all actors of a specified class.
	 *
	 * @param ActorClass The class of actors to target.
	 * @param Action The action to perform on each actor.
	 */
	void _PerformActionOnAllActors(TSubclassOf<AActor> ActorClass, void (*Action)(AActor*));

	/**
	 * Sets a state on all actors of a specified class.
	 *
	 * @param ActorClass The class of actors to target.
	 * @param state The state to set.
	 * @param Action The action to perform on each actor with the state.
	 */
	void _SetStateOnAllActors(TSubclassOf<AActor> ActorClass, bool state, void (*Action)(AActor*, bool));

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param deltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float deltaTime) override;

	/**
	 * Called when the actor is constructed or properties are changed in the editor.
	 *
	 * @param Transform The transform of the actor.
	 */
	void OnConstruction(const FTransform& Transform) override;

	/**
	 * Sets up timers for dynamic weather changes.
	 */
	void SetUpTimers();

	/**
	 * Sets the weather to the specified day time and overcast type.
	 *
	 * @param time The day time type to set.
	 * @param overcast The overcast type to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetWeather(EDayTimeTypes time, EOvercastTypes overcast);

	/**
	 * Sets the rain state in the simulation.
	 *
	 * @param rain The rain type to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRain(ERainTypes rain);
};

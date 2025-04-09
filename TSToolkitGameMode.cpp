// Fill out your copyright notice in the Description page of Project Settings.

#include "TSToolkitGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SimConfig.h"
#include "MainMenu.h"
#include "CarSpawnController.h"
#include "PeriodicCarSpawnController.h"
#include "RandomCarSpawnController.h"
#include "ScreenshotController.h"

// Delete macro if testing of level isn't needed
// #define TESTING

typedef UGameplayStatics GS;

/**
 * Constructor for ATSToolkitGameMode.
 * Initializes default values and loads required classes for the main menu and weather controller.
 */
ATSToolkitGameMode::ATSToolkitGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UUserWidget> mainMenuWidgetClass(TEXT("WidgetBlueprint'/Game/BP/UI/BP_MainMenu.BP_MainMenu_C'"));
	ConstructorHelpers::FClassFinder<AWeatherController> weatherControllerClass(TEXT("Blueprint'/Game/BP/BP_WeatherController.BP_WeatherController_C'"));

	if (mainMenuWidgetClass.Succeeded())
	{
		MainMenuWidgetClass = mainMenuWidgetClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find MainMenuWidgetClass."));
	}

	if (weatherControllerClass.Succeeded())
	{
		WeatherControllerClass = weatherControllerClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find WeatherControllerClass."));
	}
}

/**
 * Called when the game starts or when the game mode is initialized.
 * Loads the main menu or the simulation level based on the current level.
 */
void ATSToolkitGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsMainMenu())
	{
		UE_LOG(LogTemp, Warning, TEXT("Is main menu"));
		LoadMainMenu();
	}
	else
	{
#ifndef TESTING
		USimConfig* Config = NewObject<USimConfig>();
		if (!Config)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create SimConfig in BeginPlay."));
			return;
		}

		Config->LoadConfig(USimConfig::ConfigFileName);
		LoadLevel(Config);

		UWorld* world = GetWorld();
		if (!world)
		{
			UE_LOG(LogTemp, Error, TEXT("World is null in BeginPlay."));
			return;
		}

		world->EnsureCollisionTreeIsBuilt();
#endif
	}
}

/**
 * Ends the current level and quits the game.
 */
void ATSToolkitGameMode::_EndLevel()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _EndLevel."));
		return;
	}

	UKismetSystemLibrary::QuitGame(world, world->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

/**
 * Called every frame to update the game mode.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ATSToolkitGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Checks if the current level is the main menu.
 *
 * @return True if the current level is the main menu, false otherwise.
 */
bool ATSToolkitGameMode::IsMainMenu() const
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in IsMainMenu."));
		return false;
	}

	FString currentLevelName = GS::GetCurrentLevelName(world, true);
	return currentLevelName == MainMenuLevelName;
}

/**
 * Loads the main menu level and sets up the UI viewport.
 */
void ATSToolkitGameMode::LoadMainMenu()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in LoadMainMenu."));
		return;
	}

	UUserWidget* mainMenuWidget = CreateWidget<UUserWidget>(world, MainMenuWidgetClass);
	if (mainMenuWidget)
	{
		_UIViewportSetup(mainMenuWidget);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu class is null in LoadMainMenu."));
	}
}

/**
 * Loads a level based on the provided simulation configuration.
 *
 * @param Config The simulation configuration to use for loading the level.
 */
void ATSToolkitGameMode::LoadLevel(USimConfig* Config)
{
	if (!Config)
	{
		UE_LOG(LogTemp, Error, TEXT("Config is null in LoadLevel."));
		return;
	}

	_LevelViewportSetup();
	_SetUpLevel(Config);
}

/**
 * Sets up the UI viewport with the specified widget.
 *
 * @param Widget The widget to display in the viewport.
 */
void ATSToolkitGameMode::_UIViewportSetup(UUserWidget* Widget)
{
	if (!Widget)
	{
		UE_LOG(LogTemp, Error, TEXT("Widget is null in _UIViewportSetup."));
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _UIViewportSetup."));
		return;
	}

	APlayerController* controller = GS::GetPlayerController(world, 0);
	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null in _UIViewportSetup."));
		return;
	}

	Widget->AddToViewport(100);
	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(Widget->TakeWidget());
	controller->SetInputMode(inputMode);
	controller->SetShowMouseCursor(true);
}

/**
 * Sets up the viewport for the level.
 */
void ATSToolkitGameMode::_LevelViewportSetup()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _LevelViewportSetup."));
		return;
	}

	APlayerController* controller = GS::GetPlayerController(world, 0);
	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null in _LevelViewportSetup."));
		return;
	}

	FInputModeGameOnly inputMode;
	controller->SetInputMode(inputMode);
	controller->SetShowMouseCursor(false);
}

/**
 * Sets up the level based on the provided simulation configuration.
 *
 * @param Config The simulation configuration to use for setting up the level.
 */
void ATSToolkitGameMode::_SetUpLevel(USimConfig* Config)
{
	if (!Config)
	{
		UE_LOG(LogTemp, Error, TEXT("Config is null in _SetUpLevel."));
		return;
	}

	_SetUpCarSpawnController(Config);
	_SetUpScreenshotController(Config);
	_SetUpWeatherController(Config);

	// End timer setup
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATSToolkitGameMode::_EndLevel, Config->SimulationDuration, false);
}

/**
 * Sets up the car spawn controller based on the provided simulation configuration.
 *
 * @param Config The simulation configuration to use for setting up the car spawn controller.
 */
void ATSToolkitGameMode::_SetUpCarSpawnController(USimConfig* Config)
{
	if (!Config)
	{
		UE_LOG(LogTemp, Error, TEXT("Config is null in _SetUpCarSpawnController."));
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _SetUpCarSpawnController."));
		return;
	}

	ECarSpawnControllerClasses controllerClassName = Config->ControllerClassName;
	ACarSpawnController* controller = nullptr;

	if (controllerClassName == ECarSpawnControllerClasses::Random)
	{
		controller = world->SpawnActor<ARandomCarSpawnController>(ARandomCarSpawnController::StaticClass());
	}
	else if (controllerClassName == ECarSpawnControllerClasses::Periodic)
	{
		controller = world->SpawnActor<APeriodicCarSpawnController>(APeriodicCarSpawnController::StaticClass());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid controller class name in _SetUpCarSpawnController."));
		return;
	}

	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn car spawn controller in _SetUpCarSpawnController."));
		return;
	}

	controller->bRegisterAllAtBeginPlay = true;
	controller->SpawnRate = Config->CarsSpawnRate;
}

/**
 * Sets up the screenshot controller based on the provided simulation configuration.
 *
 * @param Config The simulation configuration to use for setting up the screenshot controller.
 */
void ATSToolkitGameMode::_SetUpScreenshotController(USimConfig* Config)
{
	if (!Config)
	{
		UE_LOG(LogTemp, Error, TEXT("Config is null in _SetUpScreenshotController."));
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _SetUpScreenshotController."));
		return;
	}

	AScreenshotController* controller = world->SpawnActor<AScreenshotController>(AScreenshotController::StaticClass());
	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn screenshot controller in _SetUpScreenshotController."));
		return;
	}

	controller->bRegisterAllAtBeginPlay = true;
	controller->ScreenshotInterval = Config->ScreenshotInterval;
	controller->DelayBetweenScreenshots = Config->DelayBetweenScreenshots;
}

/**
 * Sets up the weather controller based on the provided simulation configuration.
 *
 * @param Config The simulation configuration to use for setting up the weather controller.
 */
void ATSToolkitGameMode::_SetUpWeatherController(USimConfig* Config)
{
	if (!Config)
	{
		UE_LOG(LogTemp, Error, TEXT("Config is null in _SetUpWeatherController."));
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in _SetUpWeatherController."));
		return;
	}

	TArray<AActor*> found;
	GS::GetAllActorsOfClass(world, AWeatherController::StaticClass(), found);

	if (found.Num() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("More than one weather controller present in level."));
		return;
	}

	AWeatherController* controller = (found.Num() > 0) ? Cast<AWeatherController>(found[0]) : nullptr;
	if (!controller)
	{
		controller = world->SpawnActor<AWeatherController>(WeatherControllerClass);
		if (!controller)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn weather controller in _SetUpWeatherController."));
			return;
		}
	}

	EDayTimeTypes dayTime = (Config->bIsNight) ? EDayTimeTypes::Night : EDayTimeTypes::Day;
	EOvercastTypes overcast = (Config->bIsOvercast) ? EOvercastTypes::Overcast : EOvercastTypes::Clear;
	ERainTypes rain = (Config->bIsRain) ? ERainTypes::Rain : ERainTypes::NoRain;

	controller->SetWeather(dayTime, overcast);
	controller->SetRain(rain);
	controller->ChangeDayTime = Config->bIsChangeDayTime;
	controller->ChangeDayTimeRate = Config->ChangeDayTimeRate;
	controller->ChangeOvercast = Config->bIsChangeOvercast;
	controller->ChangeOvercastRate = Config->ChangeOvercastRate;
	controller->ChangeRain = Config->bIsChangeRain;
	controller->ChangeRainRate = Config->ChangeRainRate;
	controller->SetUpTimers();
}

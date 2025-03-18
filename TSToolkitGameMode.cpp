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

ATSToolkitGameMode::ATSToolkitGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FClassFinder<UUserWidget> mainMenuWidgetClass(TEXT("WidgetBlueprint'/Game/BP/UI/BP_MainMenu.BP_MainMenu_C'"));
	ConstructorHelpers::FClassFinder<AWeatherController> weatherControllerClass(TEXT("Blueprint'/Game/BP/BP_WeatherController.BP_WeatherController_C'"));
	MainMenuWidgetClass = mainMenuWidgetClass.Class;
	WeatherControllerClass = weatherControllerClass.Class;
	_Timer = NewObject<UPeriodicTimer>();
}

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
		Config->LoadConfig(USimConfig::ConfigFileName);
		LoadLevel(Config);
#endif
	}
}

void ATSToolkitGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsRunning)
	{
		return;
	}
		
	_Timer->DecrementCountdown(DeltaTime);	
	if (_Timer->CountdownState())
	{
		FGenericPlatformMisc::RequestExit(false);
	}
	
}

bool ATSToolkitGameMode::IsMainMenu() const
{
	UWorld* world = GetWorld();
	FString currentLevelName = GS::GetCurrentLevelName(world, true);
	return currentLevelName == MainMenuLevelName;
}

void ATSToolkitGameMode::LoadMainMenu()
{
	UWorld* world = GetWorld();
	UUserWidget* mainMenuWidget = CreateWidget<UUserWidget>(world, MainMenuWidgetClass);
	if (mainMenuWidget)
	{
		_UIViewportSetup(mainMenuWidget);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu class is null"));
	}
}

void ATSToolkitGameMode::LoadLevel(USimConfig* Config)
{
	_LevelViewportSetup();
	UWorld* world = GetWorld();
	_SetUpLevel(Config);
	bIsRunning = true;
}

void ATSToolkitGameMode::_UIViewportSetup(UUserWidget* Widget)
{
	UWorld* world = GetWorld();
	APlayerController* controller = GS::GetPlayerController(world, 0);
	Widget->AddToViewport(100);
	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(Widget->TakeWidget());
	controller->SetInputMode(inputMode);
	controller->SetShowMouseCursor(true);
}

void ATSToolkitGameMode::_LevelViewportSetup()
{
	UWorld* world = GetWorld();
	APlayerController* controller = GS::GetPlayerController(world, 0);
	FInputModeGameOnly inputMode;
	controller->SetInputMode(inputMode);
	controller->SetShowMouseCursor(false);
}

void ATSToolkitGameMode::_SetUpLevel(USimConfig* Config)
{
	_SetUpCarSpawnController(Config);
	_SetUpScreenshotController(Config);
	_SetUpWeatherController(Config);
	_Timer->SetInitValue(Config->SimulationDuration);
}

void ATSToolkitGameMode::_SetUpCarSpawnController(USimConfig* Config)
{
	UWorld* world = GetWorld();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, world->GetMapName());
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
		UE_LOG(LogTemp, Warning, TEXT("Invalid controller class name"));
		return;
	}
	controller->bRegisterAllAtBeginPlay = true;
	controller->SpawnRate = Config->CarsSpawnRate;
}

void ATSToolkitGameMode::_SetUpScreenshotController(USimConfig* Config)
{
	UWorld* world = GetWorld();
	AScreenshotController* controller = world->SpawnActor<AScreenshotController>(AScreenshotController::StaticClass());
	controller->bRegisterAllAtBeginPlay = true;
	controller->ScreenshotInterval = Config->ScreenshotInterval;
	controller->DelayBetweenScreenshots = Config->DelayBetweenScreenshots;
}

void ATSToolkitGameMode::_SetUpWeatherController(USimConfig* Config)
{
	TArray<AActor*> found;
	GS::GetAllActorsOfClass(GetWorld(), AWeatherController::StaticClass(), found);
	if (found.Num() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("More than one weather controller present in level"));
		return;
	}

	AWeatherController* controller = Cast<AWeatherController>(found[0]);
	if (!controller)
	{
		controller = GetWorld()->SpawnActor<AWeatherController>(WeatherControllerClass);
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
}

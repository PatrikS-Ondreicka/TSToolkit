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

typedef UGameplayStatics GS;

ATSToolkitGameMode::ATSToolkitGameMode()
{
	ConstructorHelpers::FClassFinder<UUserWidget> menuWidgetClass(TEXT("WidgetBlueprint'/Game/BP/UI/BP_MainMenu.BP_MainMenu_C'"));
	MainMenuWidgetClass = menuWidgetClass.Class;
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
		USimConfig* config = NewObject<USimConfig>();
		config->LoadConfig(USimConfig::ConfigFileName);
		LoadLevel(config);
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

void ATSToolkitGameMode::LoadLevel(USimConfig* config)
{
	_levelVieportSetup();
	UWorld* world = GetWorld();
	_setUpLevel(config);
}

void ATSToolkitGameMode::_UIViewportSetup(UUserWidget* widget)
{
	UWorld* world = GetWorld();
	APlayerController* controller = GS::GetPlayerController(world, 0);
	widget->AddToViewport(100);
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(widget->TakeWidget());
	controller->SetInputMode(InputMode);
	controller->SetShowMouseCursor(true);
}

void ATSToolkitGameMode::_levelVieportSetup()
{
	UWorld* world = GetWorld();
	APlayerController* controller = GS::GetPlayerController(world, 0);
	FInputModeGameOnly InputMode;
	controller->SetInputMode(InputMode);
	controller->SetShowMouseCursor(false);
}

void ATSToolkitGameMode::_setUpLevel(USimConfig* config)
{
	_setUpCarSpawnController(config);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("spawn config done"));
	_setUpScreenshotController(config);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("screenshot config done"));
}

void ATSToolkitGameMode::_setUpCarSpawnController(USimConfig* config)
{
	UWorld* world = GetWorld();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, world->GetMapName());
	ECarSpawnControllerClasses controllerClassName = config->ControllerClassName;
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
	controller->RegisterAllAtBeginPlay = true;
	controller->SpawnRate = config->CarsSpawnRate;
}

void ATSToolkitGameMode::_setUpScreenshotController(USimConfig* config)
{
	UWorld* world = GetWorld();
	AScreenshotController* controller = world->SpawnActor<AScreenshotController>(AScreenshotController::StaticClass());
	controller->RegisterAllAtBeginPlay = true;
	controller->ScreenshotInterval = config->ScreenshotInterval;
	controller->DelayBetweenScreenshots = config->DelayBetweenScreenshots;
}

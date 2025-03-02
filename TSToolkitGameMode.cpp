// Fill out your copyright notice in the Description page of Project Settings.


#include "TSToolkitGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SimConfig.h"
#include "MainMenu.h"

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
		// Load the level
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

void ATSToolkitGameMode::LoadLevel(FString levelName)
{
	_levelVieportSetup();
	UWorld* world = GetWorld();
	UGameplayStatics::OpenLevel(world, FName(*levelName), true);
	USimConfig* config = NewObject<USimConfig>();
	config->LoadConfig(USimConfig::ConfigFileName);
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

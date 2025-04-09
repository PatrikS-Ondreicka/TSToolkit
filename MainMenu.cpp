// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "TSToolkitGameMode.h"
#include "Kismet/GameplayStatics.h"

typedef UGameplayStatics GS;

/**
 * Constructor for UMainMenu.
 * Initializes the simulation configuration object.
 *
 * @param ObjectInitializer The object initializer for the widget.
 */
UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	_SimConfig = NewObject<USimConfig>();
	if (!_SimConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SimConfig in UMainMenu constructor."));
	}
}

/**
 * Handles the Start button click event.
 * Starts the simulation by opening the configured level.
 */
void UMainMenu::StartButtonClick()
{
	if (!_SimConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("SimConfig is null in StartButtonClick."));
		return;
	}

	FString levelPath = _SimConfig->GetLevelPath();
	if (levelPath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Level path is empty in StartButtonClick."));
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in StartButtonClick."));
		return;
	}

	AGameModeBase* gameMode = GS::GetGameMode(world);
	if (!gameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode is null in StartButtonClick."));
		return;
	}

	ATSToolkitGameMode* tsToolkitGameMode = Cast<ATSToolkitGameMode>(gameMode);
	if (!tsToolkitGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast GameMode to ATSToolkitGameMode in StartButtonClick."));
		return;
	}

	_SimConfig->SaveConfig();
	GS::OpenLevel(world, FName(*levelPath), false);
}

/**
 * Retrieves the list of available levels.
 *
 * @return An array of level names as strings.
 */
TArray<FString> UMainMenu::GetLevels() const
{
	if (!USimConfig::GetLevelNames().Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("No levels found in GetLevels."));
	}
	return USimConfig::GetLevelNames();
}

/**
 * Retrieves the list of available car spawn controller classes.
 *
 * @return An array of car spawn controller class names as strings.
 */
TArray<FString> UMainMenu::GetCarSpawnControllerClasses() const
{
	if (!USimConfig::GetCarSpawnControllerClassesNames().Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("No car spawn controller classes found in GetCarSpawnControllerClasses."));
	}
	return USimConfig::GetCarSpawnControllerClassesNames();
}

/**
 * Saves the current simulation configuration.
 */
void UMainMenu::SaveConfig()
{
	if (!_SimConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("SimConfig is null in SaveConfig."));
		return;
	}

	_SimConfig->SaveConfig();
}

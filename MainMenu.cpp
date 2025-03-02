// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "TSToolkitGameMode.h"
#include "Kismet/GameplayStatics.h"

typedef UGameplayStatics GS;

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	_simConfig = NewObject<USimConfig>();
}

void UMainMenu::StartButtonClick()
{
	FString levelPath = _simConfig->GetLevelPath();
	UWorld* world = GetWorld();
	AGameModeBase* GameMode = GS::GetGameMode(world);
	ATSToolkitGameMode* tsToolkitGameMode = Cast<ATSToolkitGameMode>(GameMode);
	_simConfig->SaveConfig();
	if (tsToolkitGameMode)
	{
		tsToolkitGameMode->LoadLevel(levelPath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game mode is null"));
	}
}

TArray<FString> UMainMenu::GetLevels()
{
	return USimConfig::GetLevelNames();
}

TArray<FString> UMainMenu::GetCarSpawnControllerClasses()
{
	return USimConfig::GetCarSpawnControllerClassesNames();
}

void UMainMenu::SaveConfig()
{
	_simConfig->SaveConfig();
}

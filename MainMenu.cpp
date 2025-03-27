// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "TSToolkitGameMode.h"
#include "Kismet/GameplayStatics.h"

typedef UGameplayStatics GS;

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	_SimConfig = NewObject<USimConfig>();
}

void UMainMenu::StartButtonClick()
{
	FString levelPath = _SimConfig->GetLevelPath();
	UWorld* world = GetWorld();
	AGameModeBase* gameMode = GS::GetGameMode(world);
	ATSToolkitGameMode* tsToolkitGameMode = Cast<ATSToolkitGameMode>(gameMode);
	_SimConfig->SaveConfig();
	if (tsToolkitGameMode)
	{
		GS::OpenLevel(world, FName(*levelPath), false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game mode is null"));
	}
}

TArray<FString> UMainMenu::GetLevels() const
{
	return USimConfig::GetLevelNames();
}

TArray<FString> UMainMenu::GetCarSpawnControllerClasses() const
{
	return USimConfig::GetCarSpawnControllerClassesNames();
}

void UMainMenu::SaveConfig()
{
	_SimConfig->SaveConfig();
}
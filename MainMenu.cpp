// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"

typedef UGameplayStatics GS;

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	_simConfig = NewObject<USimConfig>();
}

void UMainMenu::StartButtonClick()
{
	GS::OpenLevel(this, FName(_simConfig->GetLevelPath()), true, TEXT("game='/Script/TSToolkit.TSToolkitGameModeBase'"));
}

TArray<FString> UMainMenu::GetLevels()
{
	return USimConfig::GetLevelNames();
}

TArray<FString> UMainMenu::GetCarSpawnControllerClasses()
{
	return USimConfig::GetCarSpawnControllerClassesNames();
}

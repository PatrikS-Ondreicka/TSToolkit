// Fill out your copyright notice in the Description page of Project Settings.


#include "SimConfig.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

const FString USimConfig::LevelDirrPath = "/Game/SimBlank/Levels/";
const FString USimConfig::_MainMenuLevelName = "MainMenu";

USimConfig::USimConfig()
{
	RelativeLevelPath = "TCross_1.TCross_1";
	SimulationDuration = 10.0f;
	ControllerClassName = ECarSpawnControllerClasses::Random;
	CarsSpawnRate = 10.0f;
	ScreenshotInterval = 5.0f;
	DelayBetweenScreenshots = 0.2f;
}

TArray<FString> USimConfig::GetLevelNames()
{
	TArray<FString> levelNames;
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	TArray<FAssetData> assetData;
	assetRegistry.GetAssetsByPath(FName(USimConfig::LevelDirrPath), assetData);

	for (FAssetData data : assetData)
	{
		FString levelName = data.AssetName.ToString();
		if (levelName == USimConfig::_MainMenuLevelName)
		{
			continue;
		}
		levelNames.Add(levelName);
	}

	return levelNames;
}

TArray<FString> USimConfig::GetCarSpawnControllerClassesNames()
{
	TArray<FString> names = { "Random", "Periodic" };
	return names;
}

ECarSpawnControllerClasses USimConfig::GetCarSpawnControllerClassByName(FString name)
{
	if (name == "Random")
	{
		return ECarSpawnControllerClasses::Random;
	}
	else if (name == "Periodic")
	{
		return ECarSpawnControllerClasses::Periodic;
	}

	throw "Invalid CarSpawnController class name";
}

FString USimConfig::GetCarSpawnControllerClassString(ECarSpawnControllerClasses className)
{
	if (className == ECarSpawnControllerClasses::Random)
	{
		return "Random";
	}
	else if (className == ECarSpawnControllerClasses::Periodic)
	{
		return "Periodic";
	}

	throw "Invalid CarSpawnController class name";
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "SimConfig.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "Misc/Paths.h"

const FString USimConfig::LevelDirPath = "/Game/SimBlank/Levels/";
const FString USimConfig::_MainMenuLevelName = "MainMenu";
const FString USimConfig::ConfigFileName = "SimConfig.json";
const FString USimConfig::ConfigDirPath = FPaths::ProjectDir() + "Configs/";

USimConfig::USimConfig()
{
	RelativeLevelPath = "TCross_1.TCross_1";
	ControllerClassName = ECarSpawnControllerClasses::Random;
	SimulationDuration = 360.0f;
	CarsSpawnRate = 5.0f;
	ScreenshotInterval = 10.0f;
	DelayBetweenScreenshots = 0.2f;
	bIsNight = false;
	bIsOvercast = false;
	bIsRain = false;
	bIsChangeDayTime = false;
	ChangeDayTimeRate = 60.0f;
	bIsChangeOvercast = false;
	ChangeOvercastRate = 60.0f;
	bIsChangeRain = false;
	ChangeRainRate = 60.0f;
}

TArray<FString> USimConfig::GetLevelNames()
{
	TArray<FString> levelNames;
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	TArray<FAssetData> assetData;
	assetRegistry.GetAssetsByPath(FName(USimConfig::LevelDirPath), assetData);

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

void USimConfig::SaveConfig()
{
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
	jsonObject->SetStringField(TEXT("RelativeLevelPath"), RelativeLevelPath);
	jsonObject->SetNumberField(TEXT("SimulationDuration"), SimulationDuration);
	jsonObject->SetStringField(TEXT("ControllerClassName"), GetCarSpawnControllerClassString(ControllerClassName));
	jsonObject->SetNumberField(TEXT("CarsSpawnRate"), CarsSpawnRate);
	jsonObject->SetNumberField(TEXT("ScreenshotInterval"), ScreenshotInterval);
	jsonObject->SetNumberField(TEXT("DelayBetweenScreenshots"), DelayBetweenScreenshots);
	jsonObject->SetBoolField(TEXT("IsNight"), bIsNight);
	jsonObject->SetBoolField(TEXT("IsOvercast"), bIsOvercast);
	jsonObject->SetBoolField(TEXT("IsRain"), bIsRain);
	jsonObject->SetBoolField(TEXT("IsChangeDayTime"), bIsChangeDayTime);
	jsonObject->SetNumberField(TEXT("ChangeDayTimeRate"), ChangeDayTimeRate);
	jsonObject->SetBoolField(TEXT("IsChangeOvercast"), bIsChangeOvercast);
	jsonObject->SetNumberField(TEXT("ChangeOvercastRate"), ChangeOvercastRate);
	jsonObject->SetBoolField(TEXT("IsChangeRain"), bIsChangeRain);
	jsonObject->SetNumberField(TEXT("ChangeRainRate"), ChangeRainRate);

	FString jsonString;
	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<>::Create(&jsonString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWriter);
	FString saveFilePath = ConfigDirPath + ConfigFileName;
	FFileHelper::SaveStringToFile(jsonString, *saveFilePath);
}

void USimConfig::LoadConfig(FString filename)
{
	FString loadFilePath = ConfigDirPath + filename;
	FString jsonString;
	FFileHelper::LoadFileToString(jsonString, *loadFilePath);

	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(jsonString);
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		RelativeLevelPath = jsonObject->GetStringField(TEXT("RelativeLevelPath"));
		SimulationDuration = jsonObject->GetNumberField(TEXT("SimulationDuration"));
		ControllerClassName = GetCarSpawnControllerClassByName(jsonObject->GetStringField(TEXT("ControllerClassName")));
		CarsSpawnRate = jsonObject->GetNumberField(TEXT("CarsSpawnRate"));
		ScreenshotInterval = jsonObject->GetNumberField(TEXT("ScreenshotInterval"));
		DelayBetweenScreenshots = jsonObject->GetNumberField(TEXT("DelayBetweenScreenshots"));
		bIsNight = jsonObject->GetBoolField(TEXT("IsNight"));
		bIsOvercast = jsonObject->GetBoolField(TEXT("IsOvercast"));
		bIsRain = jsonObject->GetBoolField(TEXT("IsRain"));
		bIsChangeDayTime = jsonObject->GetBoolField(TEXT("IsChangeDayTime"));
		ChangeDayTimeRate = jsonObject->GetNumberField(TEXT("ChangeDayTimeRate"));
		bIsChangeOvercast = jsonObject->GetBoolField(TEXT("IsChangeOvercast"));
		ChangeOvercastRate = jsonObject->GetNumberField(TEXT("ChangeOvercastRate"));
		bIsChangeRain = jsonObject->GetBoolField(TEXT("IsChangeRain"));
		ChangeRainRate = jsonObject->GetNumberField(TEXT("ChangeRainRate"));
	}
}
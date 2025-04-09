// Fill out your copyright notice in the Description page of Project Settings.

#include "SimConfig.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "Misc/Paths.h"

// Static member initialization
const FString USimConfig::LevelDirPath = "/Game/SimBlank/Levels/";
const FString USimConfig::_MainMenuLevelName = "MainMenu";
const FString USimConfig::ConfigFileName = "SimConfig.json";
const FString USimConfig::ConfigDirPath = FPaths::ProjectDir() + "Configs/";

/**
 * Constructor for USimConfig.
 * Initializes default values for the simulation configuration.
 */
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

/**
 * Retrieves the names of all available levels in the simulation.
 *
 * @return An array of level names as strings.
 */
TArray<FString> USimConfig::GetLevelNames()
{
	TArray<FString> levelNames;

	// Load the Asset Registry module
	if (!FModuleManager::Get().IsModuleLoaded(TEXT("AssetRegistry")))
	{
		UE_LOG(LogTemp, Error, TEXT("AssetRegistry module is not loaded."));
		return levelNames;
	}

	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	// Retrieve assets in the level directory
	TArray<FAssetData> assetData;
	if (!assetRegistry.GetAssetsByPath(FName(USimConfig::LevelDirPath), assetData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve assets from path: %s"), *USimConfig::LevelDirPath);
		return levelNames;
	}

	// Filter out the main menu level and add other levels to the list
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

/**
 * Retrieves the names of all available car spawn controller classes.
 *
 * @return An array of car spawn controller class names as strings.
 */
TArray<FString> USimConfig::GetCarSpawnControllerClassesNames()
{
	return { "Random", "Periodic" };
}

/**
 * Retrieves the car spawn controller class enum value by its name.
 *
 * @param name The name of the car spawn controller class.
 * @return The corresponding ECarSpawnControllerClasses enum value.
 * @throws An exception if the name is invalid.
 */
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

	UE_LOG(LogTemp, Error, TEXT("Invalid CarSpawnController class name: %s"), *name);
	throw "Invalid CarSpawnController class name";
}

/**
 * Retrieves the name of a car spawn controller class as a string.
 *
 * @param className The car spawn controller class enum value.
 * @return The corresponding class name as a string.
 * @throws An exception if the class name is invalid.
 */
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

	UE_LOG(LogTemp, Error, TEXT("Invalid CarSpawnController class enum value."));
	throw "Invalid CarSpawnController class enum value";
}

/**
 * Saves the current simulation configuration to a JSON file.
 */
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
	if (!FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWriter))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize simulation configuration to JSON."));
		return;
	}

	FString saveFilePath = ConfigDirPath + ConfigFileName;
	if (!FFileHelper::SaveStringToFile(jsonString, *saveFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save simulation configuration to file: %s"), *saveFilePath);
	}
}

/**
 * Loads the simulation configuration from a JSON file.
 *
 * @param filename The name of the configuration file to load.
 */
void USimConfig::LoadConfig(FString filename)
{
	FString loadFilePath = ConfigDirPath + filename;
	FString jsonString;

	if (!FFileHelper::LoadFileToString(jsonString, *loadFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load simulation configuration file: %s"), *loadFilePath);
		return;
	}

	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(jsonString);
	if (!FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize simulation configuration JSON."));
		return;
	}

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

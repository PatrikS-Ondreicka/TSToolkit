// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TSToolkitGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API ATSToolkitGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATSToolkitGameMode();

	// Main menu details
	UPROPERTY(EditAnywhere, Category = "Main menu details")
	FString MainMenuLevelName = "MainMenu";

	UPROPERTY(EditAnywhere, Category = "Main menu details")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

public:
	virtual void BeginPlay() override;
	bool IsMainMenu() const;
	void LoadMainMenu();
	void LoadLevel(FString levelName);

private:
	void _UIViewportSetup(UUserWidget* widget);
	void _levelVieportSetup();
};

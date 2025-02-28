// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TSTOOLKIT_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();

	UPROPERTY(EditAnywhere, Category = "Gamemode details")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

public:
	virtual void BeginPlay() override;
};

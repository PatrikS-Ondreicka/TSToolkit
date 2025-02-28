// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MainMenu.h"

typedef UGameplayStatics GS;

AMainMenuGameMode::AMainMenuGameMode()
{
	ConstructorHelpers::FClassFinder<UUserWidget> menuWidgetClass(TEXT("WidgetBlueprint'/Game/BP/UI/BP_MainMenu.BP_MainMenu_C'"));
	MainMenuWidgetClass = menuWidgetClass.Class;
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	
	APlayerController* controller = GS::GetPlayerController(world, 0);
	UUserWidget* mainMenuWidget = CreateWidget<UUserWidget>(world, MainMenuWidgetClass);
	if (mainMenuWidget)
	{
		mainMenuWidget->AddToViewport(100);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(mainMenuWidget->TakeWidget());
		controller->SetInputMode(InputMode);
		controller->SetShowMouseCursor(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu class is null"));
	}
	
}

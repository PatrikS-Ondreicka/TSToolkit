// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficLightsGroupController.h"
#include "TrafficLightsGroup.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrafficLightsGroupController::ATrafficLightsGroupController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATrafficLightsGroupController::BeginPlay()
{
	Super::BeginPlay();

	if (bRegisterAllAtBeginPlay)
	{
		_RegisterAllGroups();
	}

	if (TrafficLightsGroups.Num() > 0)
	{
		_CurrentGroupIndex = 0;
		_CurrentGroupCountDown = GetCurrentGroup()->StateChangeTime;
		_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Green);
	}
}

// Called every frame
void ATrafficLightsGroupController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_CurrentGroupCountDown -= DeltaTime;
	if (_CurrentGroupCountDown <= 0)
	{
		NextGroup();
	}
}

void ATrafficLightsGroupController::NextGroup()
{
	_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Red);
	_CurrentGroupIndex = (_CurrentGroupIndex + 1) % TrafficLightsGroups.Num();
	_CurrentGroupCountDown = GetCurrentGroup()->StateChangeTime;
	_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Green);
}

void ATrafficLightsGroupController::_RegisterAllGroups()
{
	UWorld* world = GetWorld();
	TrafficLightsGroups.Empty();

	TArray<AActor*> found;

	UGameplayStatics::GetAllActorsOfClass(world, ATrafficLightsGroup::StaticClass(), found);
	for (AActor* actor : found)
	{
		ATrafficLightsGroup* group = Cast<ATrafficLightsGroup>(actor);
		if (group)
		{
			TrafficLightsGroups.Add(group);
		}
	}
}

void ATrafficLightsGroupController::_SetStateForGroup(int Index, ETrafficLightsStates State)
{
	if (!TrafficLightsGroups.IsValidIndex(Index))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid index in _SetStateForGroup"));
		return;
	}

	TrafficLightsGroups[Index]->SetGroupState(State);
}
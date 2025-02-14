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

	if (RegisterAllAtBeginPlay)
	{
		_registerAllGroups();
	}

	if (TrafficLightsGroups.Num() > 0)
	{
		_currentGroupIndex = 0;
		_currentGroupCountDown = GetCurrentGroup()->StateChangeTime;
		_setStateForGroup(_currentGroupIndex, ETrafficLightsStates::Green);
	}
}

// Called every frame
void ATrafficLightsGroupController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_currentGroupCountDown -= DeltaTime;
	if (_currentGroupCountDown <= 0)
	{
		NextGroup();
	}
}

void ATrafficLightsGroupController::NextGroup()
{
	_setStateForGroup(_currentGroupIndex, ETrafficLightsStates::Red);
	_currentGroupIndex = (_currentGroupIndex + 1) % TrafficLightsGroups.Num();
	_currentGroupCountDown = GetCurrentGroup()->StateChangeTime;
	_setStateForGroup(_currentGroupIndex, ETrafficLightsStates::Green);
}

void ATrafficLightsGroupController::_registerAllGroups()
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

void ATrafficLightsGroupController::_setStateForGroup(int Index, ETrafficLightsStates State)
{
	if (!TrafficLightsGroups.IsValidIndex(Index))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid index in _setStateForGroup"));
		return;
	}

	TrafficLightsGroups[Index]->SetGroupState(State);
}


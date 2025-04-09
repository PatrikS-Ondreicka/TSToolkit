// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficLightsGroupController.h"
#include "TrafficLightsGroup.h"
#include "Kismet/GameplayStatics.h"

/**
 * Constructor for ATrafficLightsGroupController.
 * Initializes default values for the traffic lights group controller.
 */
ATrafficLightsGroupController::ATrafficLightsGroupController()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Called when the game starts or when the actor is spawned.
 * Initializes traffic light groups and sets up timers.
 */
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
		_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Green);

		ATrafficLightsGroup* currentGroup = GetCurrentGroup();
		if (currentGroup)
		{
			_SetUpTimerGroup(currentGroup->StateChangeTime);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BeginPlay: Current group is null."));
		}
	}

	if (StateChangeDelay > 0.0f)
	{
		_DelayRunOut = false;
	}
}

/**
 * Called every frame to update the actor.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void ATrafficLightsGroupController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Advances to the next traffic light group and updates its state.
 */
void ATrafficLightsGroupController::NextGroup()
{
	if (TrafficLightsGroups.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NextGroup: No traffic light groups available."));
		return;
	}

	_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Red);
	_CurrentGroupIndex = (_CurrentGroupIndex + 1) % TrafficLightsGroups.Num();
	_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Green);
}

/**
 * Registers all traffic light groups in the simulation.
 * This method is called if bRegisterAllAtBeginPlay is true.
 */
void ATrafficLightsGroupController::_RegisterAllGroups()
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("_RegisterAllGroups: World is null."));
		return;
	}

	TrafficLightsGroups.Empty();

	TArray<AActor*> found;
	UGameplayStatics::GetAllActorsOfClass(world, ATrafficLightsGroup::StaticClass(), found);

	for (AActor* actor : found)
	{
		if (!actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("_RegisterAllGroups: Null actor found."));
			continue;
		}

		ATrafficLightsGroup* group = Cast<ATrafficLightsGroup>(actor);
		if (group)
		{
			TrafficLightsGroups.Add(group);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("_RegisterAllGroups: Failed to cast actor to ATrafficLightsGroup."));
		}
	}
}

/**
 * Sets the state for a specific traffic light group.
 *
 * @param Index The index of the group to update.
 * @param State The new state to set for the group.
 */
void ATrafficLightsGroupController::_SetStateForGroup(int Index, ETrafficLightsStates State)
{
	if (!TrafficLightsGroups.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("_SetStateForGroup: Invalid index %d."), Index);
		return;
	}

	ATrafficLightsGroup* group = TrafficLightsGroups[Index];
	if (!group)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetStateForGroup: Group at index %d is null."), Index);
		return;
	}

	group->SetGroupState(State);
}

/**
 * Handles actions to be performed when the delay timer runs out.
 */
void ATrafficLightsGroupController::_TimerDelayRunOutAction()
{
	_DelayRunOut = true;
	_TimerGroupRunOutAction();
}

/**
 * Handles actions to be performed when the group timer runs out.
 */
void ATrafficLightsGroupController::_TimerGroupRunOutAction()
{
	if (_DelayRunOut)
	{
		NextGroup();

		ATrafficLightsGroup* currentGroup = GetCurrentGroup();
		if (currentGroup)
		{
			_SetUpTimerGroup(currentGroup->StateChangeTime);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("_TimerGroupRunOutAction: Current group is null."));
		}
	}
	else
	{
		_SetStateForGroup(_CurrentGroupIndex, ETrafficLightsStates::Red);
		_SetUpDelayTimer();
	}
}

/**
 * Sets up the timer for the current group's state duration.
 *
 * @param Time The duration of the timer, in seconds.
 */
void ATrafficLightsGroupController::_SetUpTimerGroup(float Time)
{
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetUpTimerGroup: World is null."));
		return;
	}

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ATrafficLightsGroupController::_TimerGroupRunOutAction, Time, false);
}

/**
 * Sets up the delay timer for transitioning between groups.
 */
void ATrafficLightsGroupController::_SetUpDelayTimer()
{
	if (StateChangeDelay == 0)
	{
		return;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Error, TEXT("_SetUpDelayTimer: World is null."));
		return;
	}

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ATrafficLightsGroupController::_TimerDelayRunOutAction, StateChangeDelay, false);
}

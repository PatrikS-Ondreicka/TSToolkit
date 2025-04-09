// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLightsGroup.h"
#include "TrafficLightsGroupController.generated.h"

class ATrafficLightsGroup;

/**
 * ATrafficLightsGroupController manages multiple traffic light groups.
 * It handles state changes, timers, and group registration.
 */
UCLASS()
class TSTOOLKIT_API ATrafficLightsGroupController : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ATrafficLightsGroupController.
	 * Sets default values for this actor's properties.
	 */
	ATrafficLightsGroupController();

	/** List of traffic light groups managed by this controller. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	TArray<ATrafficLightsGroup*> TrafficLightsGroups;

	/** Delay between state changes for traffic light groups, in seconds. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	float StateChangeDelay = 0.0f;

	/** Whether to automatically register all traffic light groups at BeginPlay. */
	UPROPERTY(EditAnywhere, Category = "Controller Details")
	bool bRegisterAllAtBeginPlay = true;

private:
	/** Index of the currently active traffic light group. */
	int _CurrentGroupIndex = 0;

	/** Indicates whether the delay timer has run out. */
	bool _DelayRunOut = true;

protected:
	/**
	 * Called when the game starts or when the actor is spawned.
	 * Used for initialization logic.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Called every frame to update the actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Advances to the next traffic light group and updates its state.
	 */
	void NextGroup();

	/**
	 * Gets the index of the currently active traffic light group.
	 *
	 * @return The index of the current group.
	 */
	FORCEINLINE int GetCurrentGroupIndex()
	{
		return _CurrentGroupIndex;
	}

	/**
	 * Gets the currently active traffic light group.
	 *
	 * @return A pointer to the current traffic light group, or nullptr if no groups are available.
	 */
	FORCEINLINE ATrafficLightsGroup* GetCurrentGroup()
	{
		return (TrafficLightsGroups.Num() <= 0)
			? nullptr
			: TrafficLightsGroups[_CurrentGroupIndex];
	}

private:
	/**
	 * Registers all traffic light groups in the simulation.
	 * This method is called if bRegisterAllAtBeginPlay is true.
	 */
	void _RegisterAllGroups();

	/**
	 * Sets the state for a specific traffic light group.
	 *
	 * @param Index The index of the group to update.
	 * @param State The new state to set for the group.
	 */
	void _SetStateForGroup(int Index, ETrafficLightsStates State);

	/**
	 * Handles actions to be performed when the delay timer runs out.
	 */
	void _TimerDelayRunOutAction();

	/**
	 * Handles actions to be performed when the group timer runs out.
	 */
	void _TimerGroupRunOutAction();

	/**
	 * Sets up the timer for the current group's state duration.
	 *
	 * @param Time The duration of the timer, in seconds.
	 */
	void _SetUpTimerGroup(float Time);

	/**
	 * Sets up the delay timer for transitioning between groups.
	 */
	void _SetUpDelayTimer();
};

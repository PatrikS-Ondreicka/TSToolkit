// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CriticalZone.h"
#include "Car.generated.h"

/**
 * ACar is a class representing a car in the simulation.
 * It inherits from APawn and provides functionality for movement, collision handling,
 * traffic light interaction, and critical zone management.
 */
UCLASS(Blueprintable)
class TSTOOLKIT_API ACar : public APawn
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor for ACar.
	 * Sets default values for the car's properties.
	 */
	ACar();

	// Components
	/** Root component for the car's collision box. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UBoxComponent* CarBoxRoot;

	/** Component representing the safe distance box for the car. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UBoxComponent* SafeDistanceBox;

	/** Static mesh component representing the car's visual appearance. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UStaticMeshComponent* CarMeshComponent;

	/** Left spotlight component for the car. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* LeftSpotLight;

	/** Left spotlight effect component for the car. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* LeftSpotLightEffect;

	/** Right spotlight component for the car. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* RightSpotLight;

	/** Right spotlight effect component for the car. */
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* RightSpotLightEffect;

	// Car Details
	/** The static speed of the car. */
	UPROPERTY(EditAnywhere, Category = "Car Details")
	float StaticSpeed = 50;

protected:
	/** Indicates whether the car's lights are on. */
	bool _IsLightsOn = false;

	// Behavioral attributes
	/** The last traffic light the car interacted with. */
	class ATrafficLights* _LastTrafficLights;

	/** The movement priority of the car. */
	int _MovementPriority = 100;

	/** Indicates whether the car is handling a collision. */
	bool _CollisionHandlingState = false;

	/** Indicates whether the car is waiting for a critical zone. */
	bool _WaitingForCriticalZone = false;

	// Path attributes
	/** The current destination of the car. */
	FVector _CurrentDestination;

	/** Indicates whether the car has reached its destination. */
	bool _ReachedDestination = false;

	// Movement attributes
	/** The path the car is following. */
	class ACarPath* _Path = nullptr;

	/** Indicates whether the car can move. */
	bool _CanMove = true;

	/** The distance the car has traveled along the spline. */
	float _DistanceAlongSpline = 0;

private:
	/** Offset for the car's movement. */
	FVector _MovementOffset = FVector::ZeroVector;

	/** The critical zone the car is currently in. */
	ACriticalZone* _CurrentCriticalZone = nullptr;

protected:
	/**
	 * Called when the game starts or when the car is spawned.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Called every frame to update the car's behavior.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called to bind functionality to input.
	 * @param PlayerInputComponent The input component to bind to.
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Turns the car's lights on.
	 */
	void TurnLightsOn();

	/**
	 * Turns the car's lights off.
	 */
	void TurnLightsOff();

	// Inline getters
	/**
	 * Gets whether the car's lights are on.
	 * @return True if the lights are on, false otherwise.
	 */
	FORCEINLINE bool GetLightsOn() const
	{
		return _IsLightsOn;
	}

	/**
	 * Gets the last traffic light the car interacted with.
	 * @return A pointer to the last traffic light.
	 */
	FORCEINLINE class ATrafficLights* GetLastTrafficLights() const
	{
		return _LastTrafficLights;
	}

	/**
	 * Gets the movement priority of the car.
	 * @return The movement priority.
	 */
	FORCEINLINE int GetMovementPriority() const
	{
		return _MovementPriority;
	}

	/**
	 * Gets the path the car is following.
	 * @return A pointer to the car's path.
	 */
	FORCEINLINE class ACarPath* GetPath() const
	{
		return _Path;
	}

	// Inline setters
	/**
	 * Sets the initial distance the car has traveled along the spline.
	 * @param Distance The initial distance.
	 */
	FORCEINLINE void SetInitDistanceAlongSpline(float Distance)
	{
		_DistanceAlongSpline = Distance;
	}

	/**
	 * Sets the path the car is following.
	 * @param Path A pointer to the new path.
	 */
	FORCEINLINE void SetPath(ACarPath* Path)
	{
		_Path = Path;
	}

	/**
	 * Sets the car's destination.
	 * @param Destination The new destination.
	 */
	FORCEINLINE void SetDestination(FVector Destination)
	{
		_CurrentDestination = Destination;
	}

	/**
	 * Sets whether the car can move.
	 * @param NewState True if the car can move, false otherwise.
	 */
	FORCEINLINE void SetCanMove(bool NewState)
	{
		_CanMove = NewState;
	}

	/**
	 * Sets whether the car has reached its destination.
	 * @param NewValue True if the car has reached its destination, false otherwise.
	 */
	FORCEINLINE void SetReachedDestination(bool NewValue)
	{
		_ReachedDestination = NewValue;
	}

protected:
	/**
	 * Moves the car to a specified location.
	 * @param Location The target location.
	 * @param Speed The speed of movement.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	void _MoveToLocation(FVector Location, float Speed, float DeltaTime);

	/**
	 * Moves the car along a spline.
	 * @param Spline The spline component to follow.
	 * @param Speed The speed of movement.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	void _MoveAlongSpline(class USplineComponent* Spline, float Speed, float DeltaTime);

	/**
	 * Handles the beginning of interaction with a traffic light.
	 * @param TrafficLights The traffic light being interacted with.
	 */
	void _HandleTrafficLightsBegin(class ATrafficLights* TrafficLights);

	/**
	 * Handles the beginning of a collision with another car.
	 * @param OtherCar The other car involved in the collision.
	 * @param OtherComp The component of the other car involved in the collision.
	 */
	void _HandleCollisionBegin(ACar* OtherCar, class UPrimitiveComponent* OtherComp);

	/**
	 * Handles the end of a collision with another car.
	 * @param OtherCar The other car involved in the collision.
	 * @param OtherComp The component of the other car involved in the collision.
	 */
	void _HandleCollisionEnd(ACar* OtherCar, class UPrimitiveComponent* OtherComp);

	/**
	 * Handles the beginning of interaction with a critical zone.
	 * @param Zone The critical zone being interacted with.
	 */
	void _HandleCollisionCriticalZoneBegin(ACriticalZone* Zone);

	/**
	 * Handles the end of interaction with a critical zone.
	 * @param Zone The critical zone being interacted with.
	 */
	void _HandleCollisionCriticalZoneEnd(ACriticalZone* Zone);

	/**
	 * Reserves a critical zone for the car.
	 * @param Zone The critical zone to reserve.
	 */
	void _ReserveCriticalZone(ACriticalZone* Zone);

	/**
	 * Ends the reservation of a critical zone.
	 * @param Zone The critical zone to release.
	 */
	void _EndCriticalZoneReservation(ACriticalZone* Zone);

	/**
	 * Sets the movement priority of the car.
	 */
	void _SetMovementPriority();

	/**
	 * Creates a random offset for the car's movement.
	 * @return A random offset vector.
	 */
	FVector _CreateRandomOffset();

	/**
	 * Handles the beginning of an overlap with the safe box.
	 * @param OverlappedComponent The component that was overlapped.
	 * @param OtherActor The other actor involved in the overlap.
	 * @param OtherComp The other component involved in the overlap.
	 * @param OtherBodyIndex The body index of the other component.
	 * @param bFromSweep Whether the overlap was caused by a sweep.
	 * @param SweepResult The result of the sweep.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void _OnSafeBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/**
	 * Handles the end of an overlap with the safe box.
	 * @param OverlappedComponent The component that was overlapped.
	 * @param OtherActor The other actor involved in the overlap.
	 * @param OtherComp The other component involved in the overlap.
	 * @param OtherBodyIndex The body index of the other component.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void _OnEndSafeBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	/**
	 * Handles the end of an overlap with the root box.
	 * @param OverlappedComponent The component that was overlapped.
	 * @param OtherActor The other actor involved in the overlap.
	 * @param OtherComp The other component involved in the overlap.
	 * @param OtherBodyIndex The body index of the other component.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void _OnRootBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
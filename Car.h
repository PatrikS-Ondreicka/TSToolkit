// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Car.generated.h"

UCLASS(Blueprintable)
class TSTOOLKIT_API ACar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();

	// Components
	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UBoxComponent* CarBoxRoot;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UBoxComponent* SafeDistanceBox;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UStaticMeshComponent* CarMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* LeftSpotLight;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* LeftSpotLightEffect;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* RightSpotLight;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class USpotLightComponent* RightSpotLightEffect;

	// Car Details
	UPROPERTY(EditAnywhere, Category = "Car Details")
	float StaticSpeed = 50;

protected:
	bool _IsLightsOn = false;

	// Behavioral attributes
	class ATrafficLights* _LastTrafficLights;
	int _MovementPriority = 100;
	bool _CollisionHandlingState = false;

	// Path attributes
	FVector _CurrentDestination;
	bool _ReachedDestination = false;

	// Movement attributes
	class ACarPath* _Path = nullptr;
	bool _CanMove = true;
	float _DistanceAlongSpline = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TurnLightsOn();
	void TurnLightsOff();

	// Inline getters
	FORCEINLINE bool GetLightsOn() const
	{
		return _IsLightsOn;
	}

	FORCEINLINE class ATrafficLights* GetLastTrafficLights() const
	{
		return _LastTrafficLights;
	}

	FORCEINLINE int GetMovementPriority() const
	{
		return _MovementPriority;
	}

	// Inline setters
	FORCEINLINE void SetInitDistanceAlongSpline(float Distance)
	{
		_DistanceAlongSpline = Distance;
	}

	FORCEINLINE void SetPath(ACarPath* Path)
	{
		_Path = Path;
	}

	FORCEINLINE void SetDestination(FVector Destination)
	{
		_CurrentDestination = Destination;
	}

	FORCEINLINE void SetCanMove(bool NewState)
	{
		_CanMove = NewState;
	}

	FORCEINLINE void SetReachedDestination(bool NewValue)
	{
		_ReachedDestination = NewValue;
	}

protected:
	void _MoveToLocation(FVector Location, float Speed, float DeltaTime);
	void _MoveAlongSpline(class USplineComponent* Spline, float Speed, float DeltaTime);
	void _HandleTrafficLightsBegin(class ATrafficLights* TrafficLights);
	void _HandleCollisionBegin(ACar* OtherCar, class UPrimitiveComponent* OtherComp);
	void _HandleCollisionEnd(ACar* OtherCar, class UPrimitiveComponent* OtherComp);
	void _SetMovementPriority();

	UFUNCTION()
	virtual void _OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void _OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
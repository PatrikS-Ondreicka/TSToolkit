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

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UBoxComponent* CarBoxRoot;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UBoxComponent* SafeDistanceBox;

	UPROPERTY(EditAnywhere, Category = "Car Components")
	class UStaticMeshComponent* CarMeshComponent;

	UPROPERTY(EditAnywhere, Category="Car Stats")
	float StaticSpeed = 50;

private:
	class ACarPath* _path;
	FVector _currentDestination;
	bool _reachedDestination = false;

	// Movement attributes
	bool _canMove = true;
	float _distanceAlongSpline = 0;

	// Behaviour attributes
	int _movementPriority = 100;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	FORCEINLINE bool HasLargerPriority(ACar* Other)
	{
		return _movementPriority < Other->GetMovementPriority();
	}

	FORCEINLINE int GetMovementPriority()
	{
		return _movementPriority;
	}

	FORCEINLINE void SetMovementPriority(int Priority)
	{
		_movementPriority = Priority;
	}

	FORCEINLINE void SetInitDistanceAlongSpline(float Distance)
	{
		_distanceAlongSpline = Distance;
	}
	
	FORCEINLINE void SetPath(ACarPath* Path)
	{
		_path = Path;
	};

	FORCEINLINE void SetDestination(FVector Destination)
	{
		_currentDestination = Destination;
	}

	FORCEINLINE void SetCanMove(bool NewState)
	{
		_canMove = NewState;
	}

	FORCEINLINE void SetReachedDestination(bool NewValue)
	{
		_reachedDestination = NewValue;
	}

private:
	void _moveToALocation(FVector location, float speed, float deltaTime);
	void _moveAlongSpline(class USplineComponent* Spline, float Speed, float DeltaTime);

	UFUNCTION()
	virtual void _onBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void _onEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};

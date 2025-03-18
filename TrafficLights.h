// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLights.generated.h"

UENUM()
enum class ETrafficLightsStates
{
	Green,
	Orange,
	Red
};

UCLASS()
class TSTOOLKIT_API ATrafficLights : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrafficLights();

	// Traffic Lights Components
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UBoxComponent* TrafficLightsEffectBox;

	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UStaticMeshComponent* TrafficLightsMeshComponent;

	// Green light components
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class USpotLightComponent* GreenLightComponent;

	// Orange light components
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class USpotLightComponent* OrangeLightComponent;

	// Red light components
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class USpotLightComponent* RedLightComponent;

	// Traffic Lights Details
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Details")
	ETrafficLightsStates CurrentState = ETrafficLightsStates::Green;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTrafficLightsState(ETrafficLightsStates NewState);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGreen()
	{
		return CurrentState == ETrafficLightsStates::Green;
	}

	FORCEINLINE bool IsOrange()
	{
		return CurrentState == ETrafficLightsStates::Orange;
	}

	FORCEINLINE bool IsRed()
	{
		return CurrentState == ETrafficLightsStates::Red;
	}

protected:
	//UFUNCTION()
	//virtual void _OnBeginOverlap(
	//	UPrimitiveComponent* OverlappedComponent,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex,
	//	bool bFromSweep,
	//	const FHitResult& SweepResult
	//);

	//UFUNCTION()
	//virtual void _OnEndOverlap(
	//	UPrimitiveComponent* OverlappedComponent,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex
	//);

	void _SetCarsMove(bool CarsMoveValue);

};
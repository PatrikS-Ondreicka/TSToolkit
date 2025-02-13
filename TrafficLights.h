// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLights.generated.h"

UENUM()
enum class EOnLeavePriorityChange
{
	None,
	Largest
};

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

	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UStaticMeshComponent* GreenLightMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UStaticMeshComponent* OrangeLightMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Traffic Lights Components")
	class UStaticMeshComponent* RedLightMeshComponent;

	// Traffic Lights Details
	UPROPERTY(EditAnywhere, Category = "Traffic Lights Details")
	ETrafficLightsStates CurrentState = ETrafficLightsStates::Green;

	UPROPERTY(EditAnywhere, Category = "Traffic Lights Details")
	EOnLeavePriorityChange OnLeavePriorityChange = EOnLeavePriorityChange::None;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetTrafficLightsState(ETrafficLightsStates NewState);

protected:
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

	void _stateChange(ETrafficLightsStates NewState);

};

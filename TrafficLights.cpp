// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLights.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Car.h"

// Sets default values
ATrafficLights::ATrafficLights()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrafficLightsEffectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Traffic Ligths Effect Box Component"));
	SetRootComponent(TrafficLightsEffectBox);

	TrafficLightsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Traffic Lights Static Mesh"));
	TrafficLightsMeshComponent->SetupAttachment(RootComponent);

	// Lights Set Up
	GreenLightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Traffic Lights Green Light"));
	GreenLightMeshComponent->SetupAttachment(RootComponent);

	OrangeLightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Traffic Lights Orange Light"));
	OrangeLightMeshComponent->SetupAttachment(RootComponent);

	RedLightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Traffic Lights Red Light"));
	RedLightMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATrafficLights::BeginPlay()
{
	Super::BeginPlay();
	SetTrafficLightsState(CurrentState);

	// Set up of spawn check delegates
	TrafficLightsEffectBox->OnComponentBeginOverlap.AddDynamic(this, &ATrafficLights::_onBeginOverlap);
	TrafficLightsEffectBox->OnComponentEndOverlap.AddDynamic(this, &ATrafficLights::_onEndOverlap);
}

// Called every frame
void ATrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrafficLights::SetTrafficLightsState(ETrafficLightsStates NewState)
{
	CurrentState = NewState;
	GreenLightMeshComponent->SetVisibility(ETrafficLightsStates::Green == NewState);
	OrangeLightMeshComponent->SetVisibility(ETrafficLightsStates::Orange == NewState);
	RedLightMeshComponent->SetVisibility(ETrafficLightsStates::Red == NewState);
	_stateChange(NewState);
}

void ATrafficLights::_onBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACar* other = Cast<ACar>(OtherActor);
	if (!other)
	{
		return;
	}

	if (CurrentState == ETrafficLightsStates::Red)
	{
		other->SetCanMove(false);
	}

	if (OnLeavePriorityChange == EOnLeavePriorityChange::Largest)
	{
		other->SetMovementPriority(1);
	}
}

void ATrafficLights::_onEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ATrafficLights::_stateChange(ETrafficLightsStates NewState)
{
	TArray<AActor*> overlapingActor;
	TrafficLightsEffectBox->GetOverlappingActors(overlapingActor);
	for (AActor* actor : overlapingActor)
	{
		ACar* car = Cast<ACar>(actor);
		if (!car)
		{
			continue;
		}

		car->SetCanMove(NewState == ETrafficLightsStates::Green);
	}
}


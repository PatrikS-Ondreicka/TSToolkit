// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficLights.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Car.h"

// Sets default values
ATrafficLights::ATrafficLights()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrafficLightsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightsMeshComponent"));
	SetRootComponent(TrafficLightsMeshComponent);
	
	TrafficLightsEffectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TrafficLightsEffectBox"));
	TrafficLightsEffectBox->SetupAttachment(RootComponent);

	// Lights Set Up
	GreenLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("GreenLightComponent"));
	GreenLightComponent->SetupAttachment(RootComponent);

	OrangeLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("OrangeLightComponent"));
	OrangeLightComponent->SetupAttachment(RootComponent);

	RedLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("RedLightComponent"));
	RedLightComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATrafficLights::BeginPlay()
{
	Super::BeginPlay();
	SetTrafficLightsState(CurrentState);

	// Set up of spawn check delegates
	/*TrafficLightsEffectBox->OnComponentBeginOverlap.AddDynamic(this, &ATrafficLights::_OnBeginOverlap);
	TrafficLightsEffectBox->OnComponentEndOverlap.AddDynamic(this, &ATrafficLights::_OnEndOverlap);*/
}

// Called every frame
void ATrafficLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrafficLights::SetTrafficLightsState(ETrafficLightsStates NewState)
{
	CurrentState = NewState;
	GreenLightComponent->SetVisibility(ETrafficLightsStates::Green == NewState);
	OrangeLightComponent->SetVisibility(ETrafficLightsStates::Orange == NewState);
	RedLightComponent->SetVisibility(ETrafficLightsStates::Red == NewState);
	if (NewState == ETrafficLightsStates::Green)
	{
		_SetCarsMove(true);
	}
}

//void ATrafficLights::_OnBeginOverlap(
//	UPrimitiveComponent* OverlappedComponent,
//	AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,
//	int32 OtherBodyIndex,
//	bool bFromSweep,
//	const FHitResult& SweepResult)
//{
//	ACar* other = Cast<ACar>(OtherActor);
//	if (!other)
//	{
//		return;
//	}
//
//	if (CurrentState == ETrafficLightsStates::Red)
//	{
//		other->SetCanMove(false);
//	}
//}
//
//void ATrafficLights::_OnEndOverlap(
//	UPrimitiveComponent* OverlappedComponent,
//	AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,
//	int32 OtherBodyIndex)
//{
//}

void ATrafficLights::_SetCarsMove(bool CarsMoveValue)
{
	TArray<AActor*> overlappingActors;
	TrafficLightsEffectBox->GetOverlappingActors(overlappingActors);
	for (AActor* actor : overlappingActors)
	{
		ACar* car = Cast<ACar>(actor);
		if (!car)
		{
			continue;
		}

		car->SetCanMove(CarsMoveValue);
	}
}
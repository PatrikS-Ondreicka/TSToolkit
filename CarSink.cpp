// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSink.h"
#include "Car.h"
#include "Components/BoxComponent.h"

// Sets default values
ACarSink::ACarSink()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SinkBoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Root Box Component"));
	SetRootComponent(SinkBoxRoot);
}

// Called when the game starts or when spawned
void ACarSink::BeginPlay()
{
	Super::BeginPlay();

	SinkBoxRoot->OnComponentBeginOverlap.AddDynamic(this, &ACarSink::_onCarSinkBeginOverlap);
}

// Called every frame
void ACarSink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACarSink::_onCarSinkBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACar* other = Cast<ACar>(OtherActor);
	if (other != nullptr)
	{
		other->SetReachedDestination(true);
	}
}

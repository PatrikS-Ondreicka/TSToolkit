// Fill out your copyright notice in the Description page of Project Settings.


#include "DecorativeActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADecorativeActor::ADecorativeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DecorativeActorRootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Decorative Actor Root Box Component"));
	SetRootComponent(DecorativeActorRootBox);

	DecorativeActorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decorative Actor Static Mesh"));
	DecorativeActorMeshComponent->SetupAttachment(RootComponent);
	DecorativeActorMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void ADecorativeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecorativeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


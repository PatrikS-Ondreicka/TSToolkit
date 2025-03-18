// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSpawnController.h"
#include "CarSource.h"
#include "PeriodicTimer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define NO_LARGE_VEHICLES

TArray<FString> ACarSpawnController::CarBpPaths
{
#ifndef NO_LARGE_VEHICLES
	// Bus
	TEXT("/Game/BP/Cars/BP_Bus.BP_Bus_C"),
	// Delivery
	TEXT("/Game/BP/Cars/BP_Delivery.BP_Delivery_C"),
#endif
	// Coupe
	TEXT("/Game/BP/Cars/Coupe/BP_Coupe.BP_Coupe_C"),
	TEXT("/Game/BP/Cars/Coupe/BP_RedCoupe.BP_RedCoupe_C"),
	TEXT("/Game/BP/Cars/Coupe/BP_GreenCoupe.BP_GreenCoupe_C"),
	TEXT("/Game/BP/Cars/Coupe/BP_BlackCoupe.BP_BlackCoupe_C"),
	// Offroad
	TEXT("/Game/BP/Cars/Offroad/BP_Offroad.BP_Offroad_C"),
	TEXT("/Game/BP/Cars/Offroad/BP_RedOffroad.BP_RedOffroad_C"),
	TEXT("/Game/BP/Cars/Offroad/BP_YellowOffroad.BP_YellowOffroad_C"),
	TEXT("/Game/BP/Cars/Offroad/BP_BlackOffroad.BP_BlackOffroad_C"),
	// Suv
	TEXT("/Game/BP/Cars/Suv/BP_Suv.BP_Suv_C"),
	TEXT("/Game/BP/Cars/Suv/BP_RedSuv.BP_RedSuv_C"),
	TEXT("/Game/BP/Cars/Suv/BP_BrownSuv.BP_BrownSuv_C"),
	TEXT("/Game/BP/Cars/Suv/BP_BlackSuv.BP_BlackSuv_C"),
	// Van
	TEXT("/Game/BP/Cars/Van/BP_Van_C"),
	TEXT("/Game/BP/Cars/Van/BP_YellowVan.BP_YellowVan_C"),
	TEXT("/Game/BP/Cars/Van/BP_WhiteVan.BP_WhiteVan_C")
};

// Sets default values
ACarSpawnController::ACarSpawnController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (FString path : CarBpPaths)
	{
		ConstructorHelpers::FClassFinder<ACar> classFinder(*path);
		if (classFinder.Class)
		{
			CarBpPool.Add(classFinder.Class);
		}
	}

	_Timer = NewObject<UPeriodicTimer>();
	_Timer->SetInitValue(60.0f);
}

// Called when the game starts or when spawned
void ACarSpawnController::BeginPlay()
{
	Super::BeginPlay();
	_Timer->SetInitValue(SpawnRate);
	if (bRegisterAllAtBeginPlay)
	{
		_RegisterAllSources();
	}

	if (Sources.Num() > 0)
	{
		_RoundSetUp();
	}

	SetNight(_IsNight);
}

// Called every frame
void ACarSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarSpawnController::SetNight(bool State)
{
	_IsNight = State;
	for (ACarSource* source : Sources)
	{
		source->IsNight = State;
	}
}

void ACarSpawnController::_RegisterAllSources()
{
	UWorld* world = GetWorld();
	Sources.Empty();

	TArray<AActor*> found;

	UGameplayStatics::GetAllActorsOfClass(world, ACarSource::StaticClass(), found);
	for (AActor* actor : found)
	{
		ACarSource* source = Cast<ACarSource>(actor);
		if (source)
		{
			Sources.Add(source);
		}
	}
}

void ACarSpawnController::_RoundSetUp()
{
	_Timer->ResetCountdown();
}

bool ACarSpawnController::_CanSourcesSpawn()
{
	if (Sources.Num() <= 0)
	{
		return false;
	}

	for (auto carSource : Sources)
	{
		if (!carSource->GetCanSpawn())
		{
			return false;
		}
	}

	return true;
}

TSubclassOf<ACar> ACarSpawnController::_GetRandomCarClass()
{
	int randomIndex = UKismetMathLibrary::RandomInteger(CarBpPool.Num());

	if (CarBpPool.IsValidIndex(randomIndex))
	{
		return CarBpPool[randomIndex];
	}

	return nullptr;
}
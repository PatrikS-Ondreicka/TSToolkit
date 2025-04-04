// Fill out your copyright notice in the Description page of Project Settings.

#include "Car.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "CarPath.h"
#include "TrafficLights.h"
#include "Kismet/KismetMathLibrary.h"

#define MAX_MOVEMENT_PRIORITY 1000000000
#define PATH_VARIATON_HALF_RANGE 20

#define MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, str);
#define ERROR_MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, str);

// Sets default values
ACar::ACar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CarMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Static Mesh"));
	SetRootComponent(CarMeshComponent);

	CarBoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Car Root Box Component"));
	CarBoxRoot->SetupAttachment(RootComponent);

	SafeDistanceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Car Safe Distance Box"));
	SafeDistanceBox->SetupAttachment(RootComponent);

	LeftSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Car Left Spot Light"));
	LeftSpotLight->SetupAttachment(CarMeshComponent);

	LeftSpotLightEffect = CreateDefaultSubobject<USpotLightComponent>(TEXT("Car Left Spot Light Effect"));
	LeftSpotLightEffect->SetupAttachment(CarMeshComponent);

	RightSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Car Right Spot Light"));
	RightSpotLight->SetupAttachment(CarMeshComponent);

	RightSpotLightEffect = CreateDefaultSubobject<USpotLightComponent>(TEXT("Car Right Spot Light Effect"));
	RightSpotLightEffect->SetupAttachment(CarMeshComponent);

	_LastTrafficLights = nullptr;
	_Path = nullptr;

	_SetMovementPriority();
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	_MovementOffset = _CreateRandomOffset();
	SafeDistanceBox->SetGenerateOverlapEvents(true);
	SafeDistanceBox->OnComponentBeginOverlap.AddDynamic(this, &ACar::_OnSafeBoxBeginOverlap);
	SafeDistanceBox->OnComponentEndOverlap.AddDynamic(this, &ACar::_OnEndSafeBoxOverlap);
	CarBoxRoot->OnComponentEndOverlap.AddDynamic(this, &ACar::_OnRootBoxEndOverlap);
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_ReachedDestination)
	{
		K2_DestroyActor();
		return;
	}

	if (_CanMove)
	{
		if (_Path == nullptr)
		{
			ERROR_MSG("Path for a car is nullptr");
			return;
		}
		_MoveAlongSpline(_Path->Path, StaticSpeed, DeltaTime);
	}

	if (!_WaitingForCriticalZone)
	{
		return;
	}

	if (!_CurrentCriticalZone)
	{
		_WaitingForCriticalZone = false;
		return;
	}

	if (_CurrentCriticalZone->IsReserved())
	{
		return;
	}
	else
	{
		_ReserveCriticalZone(_CurrentCriticalZone);
	}
}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACar::TurnLightsOn()
{
	LeftSpotLight->SetVisibility(true);
	LeftSpotLightEffect->SetVisibility(true);
	RightSpotLight->SetVisibility(true);
	RightSpotLightEffect->SetVisibility(true);
	_IsLightsOn = true;
}

void ACar::TurnLightsOff()
{
	LeftSpotLight->SetVisibility(false);
	LeftSpotLightEffect->SetVisibility(false);
	RightSpotLight->SetVisibility(false);
	RightSpotLightEffect->SetVisibility(false);
	_IsLightsOn = false;
}

void ACar::_MoveToLocation(FVector Location, float Speed, float DeltaTime)
{
	FVector direction = (Location - GetActorLocation()).GetSafeNormal(0.1);
	FVector movementDelta = direction * Speed * DeltaTime;

	SetActorLocation(GetActorLocation() + movementDelta);
}

void ACar::_MoveAlongSpline(USplineComponent* Spline, float Speed, float DeltaTime)
{
	float newDistance = _DistanceAlongSpline + Speed * DeltaTime;
	FVector newLocation = Spline->GetLocationAtDistanceAlongSpline(newDistance, ESplineCoordinateSpace::World) + _MovementOffset;
	FRotator newRotation = Spline->GetRotationAtDistanceAlongSpline(newDistance, ESplineCoordinateSpace::World);
	SetActorLocation(newLocation);
	SetActorRotation(newRotation);
	_DistanceAlongSpline = newDistance;
}

void ACar::_HandleTrafficLightsBegin(ATrafficLights* TrafficLights)
{
	if (TrafficLights == nullptr)
	{
		return;
	}

	if (TrafficLights->IsGreen())
	{
		_LastTrafficLights = TrafficLights;
		_CanMove = true;
		return;
	}

	if (TrafficLights->IsRed() && TrafficLights != _LastTrafficLights)
	{
		_CanMove = false;
	}

}

void ACar::_HandleCollisionBegin(ACar* OtherCar, UPrimitiveComponent* OtherComp)
{
	if (OtherCar == nullptr || OtherComp == nullptr)
	{
		return;
	}

	if (_CollisionHandlingState)
	{
		_CanMove = true;
		return;
	}

	if (OtherComp != OtherCar->SafeDistanceBox)
	{
		_CanMove = false;
		return;
	}	

	if (_MovementPriority < OtherCar->GetMovementPriority())
	{
		_CollisionHandlingState = true;
		_CanMove = true;
	}
	else
	{
		_CanMove = false;
	}
}

void ACar::_HandleCollisionEnd(ACar* OtherCar, class UPrimitiveComponent* OtherComp)
{
	if (OtherCar == nullptr || OtherComp == nullptr)
	{
		return;
	}

	_CanMove = true;
	_CollisionHandlingState = false;
}

void ACar::_HandleCollisionCriticalZoneBegin(ACriticalZone* Zone)
{
	if (Zone == nullptr)
	{
		return;
	}
	if (Zone->IsReserved() && !Zone->IsReservedForPath(_Path))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Crit wait"));
		_CurrentCriticalZone = Zone;
		_WaitingForCriticalZone = true;
		_CanMove = false;
		return;
	}
	_ReserveCriticalZone(Zone);
}

void ACar::_HandleCollisionCriticalZoneEnd(ACriticalZone* Zone)
{
	if (Zone == nullptr)
	{
		return;
	}
	if (!Zone->IsReserved())
	{
		return;
	}
	Zone->TryEndReservation();
	_CanMove = true;
}

void ACar::_ReserveCriticalZone(ACriticalZone* Zone)
{
	if (Zone == nullptr)
	{
		return;
	}

	if (Zone->IsReserved())
	{
		return;
	}

	Zone->SetReserved(_Path);
	_WaitingForCriticalZone = false;
	_CanMove = true;
}

void ACar::_EndCriticalZoneReservation(ACriticalZone* Zone)
{
	if (Zone == nullptr)
	{
		return;
	}
	if (!Zone->IsReserved())
	{
		return;
	}
	Zone->SetReserved(nullptr);
	_CanMove = true;
}

void ACar::_SetMovementPriority()
{
	_MovementPriority = UKismetMathLibrary::RandomInteger(MAX_MOVEMENT_PRIORITY);
}

FVector ACar::_CreateRandomOffset()
{
	return FVector(FMath::RandRange(-PATH_VARIATON_HALF_RANGE, PATH_VARIATON_HALF_RANGE),
		FMath::RandRange(-PATH_VARIATON_HALF_RANGE, PATH_VARIATON_HALF_RANGE), 0.0f);
}

void ACar::_OnSafeBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACar* otherCar = Cast<ACar>(OtherActor);
	if (otherCar != nullptr && otherCar != this)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("{%p} - Collision"), this));
		_HandleCollisionBegin(otherCar, OtherComp);
		return;
	}

	ATrafficLights* otherTf = Cast<ATrafficLights>(OtherActor);
	if (otherTf != nullptr)
	{
		_HandleTrafficLightsBegin(otherTf);
	}

	ACriticalZone* otherZone = Cast<ACriticalZone>(OtherActor);
	if (otherZone != nullptr)
	{
		_HandleCollisionCriticalZoneBegin(otherZone);
	}
}

void ACar::_OnEndSafeBoxOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ACar* other = Cast<ACar>(OtherActor);
	if (!other || other == this)
	{
		return;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("{%p} - Collision end"), this));
	_HandleCollisionEnd(other, OtherComp);
	
}

void ACar::_OnRootBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ACriticalZone* otherZone = Cast<ACriticalZone>(OtherActor);
	if (otherZone != nullptr)
	{
		_HandleCollisionCriticalZoneEnd(otherZone);
	}
}

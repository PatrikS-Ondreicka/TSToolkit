
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
#define PATH_VARIATION_HALF_RANGE 20

/**
 * Constructor for ACar.
 * Sets default values for the car's components and initializes movement priority.
 */
ACar::ACar()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
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

/**
 * Called when the game starts or when the car is spawned.
 * Initializes random movement offset and sets up collision event bindings.
 */
void ACar::BeginPlay()
{
	Super::BeginPlay();

	_MovementOffset = _CreateRandomOffset();

	if (!SafeDistanceBox || !CarBoxRoot)
	{
		UE_LOG(LogTemp, Error, TEXT("SafeDistanceBox or CarBoxRoot is null in BeginPlay."));
		return;
	}

	SafeDistanceBox->SetGenerateOverlapEvents(true);
	SafeDistanceBox->OnComponentBeginOverlap.AddDynamic(this, &ACar::_OnSafeBoxBeginOverlap);
	SafeDistanceBox->OnComponentEndOverlap.AddDynamic(this, &ACar::_OnEndSafeBoxOverlap);
	CarBoxRoot->OnComponentEndOverlap.AddDynamic(this, &ACar::_OnRootBoxEndOverlap);
}

/**
 * Called every frame to update the car's behavior.
 * Handles movement, path following, and critical zone interactions.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
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
		if (!_Path)
		{
			UE_LOG(LogTemp, Error, TEXT("Path for a car is nullptr in Tick."));
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

/**
 * Called to bind functionality to input.
 *
 * @param PlayerInputComponent The input component to bind to.
 */
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/**
 * Turns the car's lights on.
 */
void ACar::TurnLightsOn()
{
	if (!LeftSpotLight || !LeftSpotLightEffect || !RightSpotLight || !RightSpotLightEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("SpotLight components are null in TurnLightsOn."));
		return;
	}

	LeftSpotLight->SetVisibility(true);
	LeftSpotLightEffect->SetVisibility(true);
	RightSpotLight->SetVisibility(true);
	RightSpotLightEffect->SetVisibility(true);
	_IsLightsOn = true;
}

/**
 * Turns the car's lights off.
 */
void ACar::TurnLightsOff()
{
	if (!LeftSpotLight || !LeftSpotLightEffect || !RightSpotLight || !RightSpotLightEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("SpotLight components are null in TurnLightsOff."));
		return;
	}

	LeftSpotLight->SetVisibility(false);
	LeftSpotLightEffect->SetVisibility(false);
	RightSpotLight->SetVisibility(false);
	RightSpotLightEffect->SetVisibility(false);
	_IsLightsOn = false;
}

/**
 * Moves the car to a specified location.
 *
 * @param Location The target location.
 * @param Speed The speed of movement.
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACar::_MoveToLocation(FVector Location, float Speed, float DeltaTime)
{
	FVector direction = (Location - GetActorLocation()).GetSafeNormal(0.1f);
	FVector movementDelta = direction * Speed * DeltaTime;

	SetActorLocation(GetActorLocation() + movementDelta);
}

/**
 * Moves the car along a spline.
 *
 * @param Spline The spline component to follow.
 * @param Speed The speed of movement.
 * @param DeltaTime The time elapsed since the last frame.
 */
void ACar::_MoveAlongSpline(USplineComponent* Spline, float Speed, float DeltaTime)
{
	if (!Spline)
	{
		UE_LOG(LogTemp, Error, TEXT("Spline is null in _MoveAlongSpline."));
		return;
	}

	float newDistance = _DistanceAlongSpline + Speed * DeltaTime;
	FVector newLocation = Spline->GetLocationAtDistanceAlongSpline(newDistance, ESplineCoordinateSpace::World) + _MovementOffset;
	FRotator newRotation = Spline->GetRotationAtDistanceAlongSpline(newDistance, ESplineCoordinateSpace::World);
	SetActorLocation(newLocation);
	SetActorRotation(newRotation);
	_DistanceAlongSpline = newDistance;
}

/**
 * Handles the beginning of interaction with a traffic light.
 *
 * @param TrafficLights The traffic light being interacted with.
 */
void ACar::_HandleTrafficLightsBegin(ATrafficLights* TrafficLights)
{
	if (!TrafficLights)
	{
		UE_LOG(LogTemp, Error, TEXT("TrafficLights is null in _HandleTrafficLightsBegin."));
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

/**
 * Handles the beginning of a collision with another car.
 *
 * @param OtherCar The other car involved in the collision.
 * @param OtherComp The component of the other car involved in the collision.
 */
void ACar::_HandleCollisionBegin(ACar* OtherCar, UPrimitiveComponent* OtherComp)
{
	if (!OtherCar || !OtherComp)
	{
		UE_LOG(LogTemp, Error, TEXT("OtherCar or OtherComp is null in _HandleCollisionBegin."));
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

/**
 * Handles the end of a collision with another car.
 *
 * @param OtherCar The other car involved in the collision.
 * @param OtherComp The component of the other car involved in the collision.
 */
void ACar::_HandleCollisionEnd(ACar* OtherCar, UPrimitiveComponent* OtherComp)
{
	if (!OtherCar || !OtherComp)
	{
		UE_LOG(LogTemp, Error, TEXT("OtherCar or OtherComp is null in _HandleCollisionEnd."));
		return;
	}

	_CanMove = true;
	_CollisionHandlingState = false;
}

/**
 * Handles the beginning of interaction with a critical zone.
 *
 * @param Zone The critical zone being interacted with.
 */
void ACar::_HandleCollisionCriticalZoneBegin(ACriticalZone* Zone)
{
	if (!Zone)
	{
		UE_LOG(LogTemp, Error, TEXT("Zone is null in _HandleCollisionCriticalZoneBegin."));
		return;
	}

	if (Zone->IsReserved() && !Zone->IsReservedForPath(_Path))
	{
		UE_LOG(LogTemp, Warning, TEXT("Critical zone is reserved. Waiting for reservation to end."));
		_CurrentCriticalZone = Zone;
		_WaitingForCriticalZone = true;
		_CanMove = false;
		return;
	}
	_ReserveCriticalZone(Zone);
}

/**
 * Handles the end of interaction with a critical zone.
 *
 * @param Zone The critical zone being interacted with.
 */
void ACar::_HandleCollisionCriticalZoneEnd(ACriticalZone* Zone)
{
	if (!Zone)
	{
		UE_LOG(LogTemp, Error, TEXT("Zone is null in _HandleCollisionCriticalZoneEnd."));
		return;
	}

	if (!Zone->IsReserved())
	{
		return;
	}

	Zone->TryEndReservation();
	_CanMove = true;
}

/**
 * Reserves a critical zone for the car.
 *
 * @param Zone The critical zone to reserve.
 */
void ACar::_ReserveCriticalZone(ACriticalZone* Zone)
{
	if (!Zone)
	{
		UE_LOG(LogTemp, Error, TEXT("Zone is null in _ReserveCriticalZone."));
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

/**
 * Ends the reservation of a critical zone.
 *
 * @param Zone The critical zone to release.
 */
void ACar::_EndCriticalZoneReservation(ACriticalZone* Zone)
{
	if (!Zone)
	{
		UE_LOG(LogTemp, Error, TEXT("Zone is null in _EndCriticalZoneReservation."));
		return;
	}

	if (!Zone->IsReserved())
	{
		return;
	}

	Zone->SetReserved(nullptr);
	_CanMove = true;
}

/**
 * Sets the movement priority of the car to a random value.
 */
void ACar::_SetMovementPriority()
{
	_MovementPriority = UKismetMathLibrary::RandomInteger(MAX_MOVEMENT_PRIORITY);
}

/**
 * Creates a random offset for the car's movement.
 *
 * @return A random offset vector.
 */
FVector ACar::_CreateRandomOffset()
{
	return FVector(FMath::RandRange(-PATH_VARIATION_HALF_RANGE, PATH_VARIATION_HALF_RANGE),
		FMath::RandRange(-PATH_VARIATION_HALF_RANGE, PATH_VARIATION_HALF_RANGE), 0.0f);
}

/**
 * Handles the beginning of an overlap with the safe box.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The other actor involved in the overlap.
 * @param OtherComp The other component involved in the overlap.
 * @param OtherBodyIndex The body index of the other component.
 * @param bFromSweep Whether the overlap was caused by a sweep.
 * @param SweepResult The result of the sweep.
 */
void ACar::_OnSafeBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACar* otherCar = Cast<ACar>(OtherActor);
	if (otherCar && otherCar != this)
	{
		_HandleCollisionBegin(otherCar, OtherComp);
		return;
	}

	ATrafficLights* otherTf = Cast<ATrafficLights>(OtherActor);
	if (otherTf)
	{
		_HandleTrafficLightsBegin(otherTf);
	}

	ACriticalZone* otherZone = Cast<ACriticalZone>(OtherActor);
	if (otherZone)
	{
		_HandleCollisionCriticalZoneBegin(otherZone);
	}
}

/**
 * Handles the end of an overlap with the safe box.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The other actor involved in the overlap.
 * @param OtherComp The other component involved in the overlap.
 * @param OtherBodyIndex The body index of the other component.
 */
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

	_HandleCollisionEnd(other, OtherComp);
}

/**
 * Handles the end of an overlap with the root box.
 *
 * @param OverlappedComponent The component that was overlapped.
 * @param OtherActor The other actor involved in the overlap.
 * @param OtherComp The other component involved in the overlap.
 * @param OtherBodyIndex The body index of the other component.
 */
void ACar::_OnRootBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ACriticalZone* otherZone = Cast<ACriticalZone>(OtherActor);
	if (otherZone)
	{
		_HandleCollisionCriticalZoneEnd(otherZone);
	}
}

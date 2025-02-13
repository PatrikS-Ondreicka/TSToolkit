// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "CarPath.h"

#define MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(str));
#define ERROR_MSG(str) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(str));

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CarBoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Car Root Box Component"));
	SetRootComponent(CarBoxRoot);

	SafeDistanceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Car Safe Distance Box"));
	SafeDistanceBox->SetupAttachment(RootComponent);

	CarMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Static Mesh"));
	CarMeshComponent->SetupAttachment(RootComponent);

	_path = nullptr;
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();

	SafeDistanceBox->OnComponentBeginOverlap.AddDynamic(this, &ACar::_onBeginOverlap);
	SafeDistanceBox->OnComponentEndOverlap.AddDynamic(this, &ACar::_onEndOverlap);
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_reachedDestination)
	{
		K2_DestroyActor();
		return;
	}

	if (_canMove)
	{
		if (_path == nullptr)
		{
			ERROR_MSG("Path for a car is nullptr");
			return;
		}
		_moveAlongSpline(_path->Path, StaticSpeed, DeltaTime);
		// _moveToALocation(_currentDestination, StaticSpeed, DeltaTime);
	}
}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACar::_moveToALocation(FVector location, float speed, float deltaTime)
{
	FVector direction = (location - GetActorLocation()).GetSafeNormal(0.1);
	FVector movementDelta = direction * speed * deltaTime;

	SetActorLocation(GetActorLocation() + movementDelta);
}

void ACar::_moveAlongSpline(USplineComponent* Spline, float Speed, float DeltaTime)
{
	float newDistance = _distanceAlongSpline + Speed * DeltaTime;
	FVector newLocation = Spline->GetLocationAtDistanceAlongSpline(newDistance, ESplineCoordinateSpace::World);
	FRotator newRotation = Spline->GetRotationAtDistanceAlongSpline(newDistance, ESplineCoordinateSpace::World);
	SetActorLocation(newLocation);
	SetActorRotation(newRotation);
	_distanceAlongSpline = newDistance;
}

void ACar::_onBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	ACar* other = Cast<ACar>(OtherActor);
	if (!other || other == this)
	{
		return;
	}

	if (HasLargerPriority(other))
	{
		return;
	}
	_canMove = false;
}

void ACar::_onEndOverlap(
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
	_canMove = true;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPath.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
ACarPath::ACarPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Car Path"));
	SetRootComponent(Path);

	StartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Path start"));
	StartMesh->SetupAttachment(RootComponent);

	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Path end"));
	EndMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACarPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACarPath::OnConstruction(const FTransform& Transform)
{

}

// Called every frame
void ACarPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACarPath::IsPathRelated(ACarPath* OtherPath)
{
	if (!OtherPath)
	{
		return false;
	}

	return RelatedPaths.Contains(OtherPath);
}

void ACarPath::AddPathRelation(ACarPath* OtherPath)
{
	if (!OtherPath)
	{
		return;
	}
	if (!IsPathRelated(OtherPath))
	{
		RelatedPaths.Add(OtherPath);
	}
}

void ACarPath::RemovePathRelation(ACarPath* OtherPath)
{
	if (!OtherPath)
	{
		return;
	}

	RelatedPaths.Remove(OtherPath);

}

void ACarPath::UpdateRelations()
{
	if (RelatedPaths.Num() <= 0)
	{
		return;
	}

	for (auto path : RelatedPaths)
	{
		if (!path)
		{
			continue;
		}

		if (path == this)
		{
			RemovePathRelation(this);
		}

		if (!path->IsPathRelated(this))
		{
			path->AddPathRelation(this);
		}

		if (!IsPathRelated(path))
		{
			RemovePathRelation(path);
			path->UpdateRelations();
		}
	}
}

#if WITH_EDITOR
//void ACarPath::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
//	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ACarPath, RelatedPaths))
//	{
//		UpdateRelations();
//	}
//
//	if (PropertyChangedEvent.Property 
//		&& (PropertyChangedEvent.Property->GetFName().IsEqual(*StartMesh->GetName()))
//		|| PropertyChangedEvent.Property->GetFName().IsEqual(*EndMesh->GetName()))
//	{
//		FVector startLocation = StartMesh->GetComponentLocation();
//		FVector endLocation = EndMesh->GetComponentLocation();
//		int32 pointsCount = Path->GetNumberOfSplinePoints();
//		Path->SetWorldLocationAtSplinePoint(0, startLocation);
//		Path->SetWorldLocationAtSplinePoint(pointsCount - 1, endLocation);
//	}
//}
#endif


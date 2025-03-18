// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.generated.h"

UENUM()
enum class ERoadType
{
	OneWay,
	TwoWay
};

UCLASS()
class TSTOOLKIT_API ARoad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoad();

	UPROPERTY(EditAnywhere, Category = "Road Components")
	class USplineComponent* RoadSpline;

	UPROPERTY(EditAnywhere, Category = "Road Components")
	class UStaticMesh* RoadMesh;

	UPROPERTY(EditAnywhere, Category = "Road Details")
	ERoadType RoadType = ERoadType::OneWay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnConstruction(const FTransform& Transform) override;

private:
	void _SetUpMesh();
};
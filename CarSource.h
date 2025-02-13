// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car.h"
#include "CarPath.h"
#include "CarSource.generated.h"

UCLASS(Blueprintable)
class TSTOOLKIT_API ACarSource : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarSource();

	// Components
	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UBoxComponent* SourceBoxRoot;

	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UStaticMeshComponent* SourceMesh;

	UPROPERTY(EditAnywhere, Category = "Source Components")
	class UBoxComponent* SpawnCheckBox;

	UPROPERTY(EditAnywhere, Category = "Source Components")
	TArray<ACarPath*> Paths;

	// Details
	UPROPERTY(EditAnywhere, Category = "Source Details")
	TSubclassOf<ACar> CarClass;

	UPROPERTY(EditAnywhere, Category = "Source Details")
	int MaxCarsCount = 5;

	UPROPERTY(EditAnywhere, Category = "Source Details")
	class ACarSpawnController* Controller = nullptr;

	// Car behaviour details
	UPROPERTY(EditAnywhere, Category = "Spawned Car Details")
	float CarStaticSpeed = 10;

	UPROPERTY(EditAnywhere, Category = "Spawned Car Details")
	int CarMovementPriority = 100;

private:
	// Spawning attributes
	bool _canSpawn = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void SpawnCar();

	// Getter and Setter for _canSpawn
	FORCEINLINE void SetCanSpawn(bool CanSpawnValue)
	{
		_canSpawn = CanSpawnValue;
	}

	FORCEINLINE bool GetCanSpawn()
	{
		return _canSpawn;
	}

private:

	UFUNCTION()
	virtual void _onSpawnCheckBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void _onSpawnCheckEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	ACarPath* _selectPath();
	void _initPath();
};

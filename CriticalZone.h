// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarPath.h"
#include "CriticalZone.generated.h"

UCLASS()
class TSTOOLKIT_API ACriticalZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACriticalZone();

	UPROPERTY(EditAnywhere, Category = "Critical Zone Components")
	class UBoxComponent* BoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ACarPath* _CurrentPath;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	FORCEINLINE bool IsReserved() const
	{
		return _CurrentPath != nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	FORCEINLINE void SetReserved(ACarPath* Path)
	{
		_CurrentPath = Path;
	}

	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	bool IsReservedForPath(ACarPath* Path);

	UFUNCTION(BlueprintCallable, Category = "Critical Zone")
	void TryEndReservation();

};

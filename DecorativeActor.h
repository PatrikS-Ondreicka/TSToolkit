// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorativeActor.generated.h"

UCLASS()
class TSTOOLKIT_API ADecorativeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecorativeActor();

	UPROPERTY(EditAnywhere, Category = "Decorative Actor Components")
	class UBoxComponent* DecorativeActorRootBox;

	UPROPERTY(EditAnywhere, Category = "Decorative Actor Components")
	class UStaticMeshComponent* DecorativeActorMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

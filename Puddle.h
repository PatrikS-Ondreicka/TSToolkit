// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorativeActor.h"
#include "Puddle.generated.h"

UCLASS()
class TSTOOLKIT_API APuddle : public ADecorativeActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarPath.generated.h"

UCLASS()
class TSTOOLKIT_API ACarPath : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarPath();

	UPROPERTY(EditAnywhere, Category = "Path Components")
	class USplineComponent* Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Components")
	class UStaticMeshComponent* StartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Components")
	class UStaticMeshComponent* EndMesh;

	UPROPERTY(EditAnywhere, Category = "Path Details")
	float Probability = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path Details")
	TArray<ACarPath*> RelatedPaths;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnConstruction(const FTransform& Transform) override;
	// void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	bool IsPathRelated(ACarPath* OtherPath);

	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	void AddPathRelation(ACarPath* OtherPath);

	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	void RemovePathRelation(ACarPath* OtherPath);

	UFUNCTION(BlueprintCallable, Category = "Path Relations")
	void UpdateRelations();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UCapsuleComponent;

class UGeometryCollectionComponent;


UCLASS()
class CASTLE_API ABreakableActor : public AActor,public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		UGeometryCollectionComponent* GemoteryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCapsuleComponent* CapsuleComponent;



	bool Bbroken = false;

private:


	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
		TArray<TSubclassOf<class ATreasure>> TreasureClasses;



};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "item.generated.h"

class USphereComponent;

class UNiagaraComponent;
class UNiagaraSystem;

enum class EItemState
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class CASTLE_API Aitem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aitem();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() 
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Sin Prameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category = "Sin Prameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename T>
	T Avg(T First, T Second);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;


	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;



private:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunTime =0;

	
	UPROPERTY(EditAnywhere)
		 UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;


};

template<typename T>
inline T Aitem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}

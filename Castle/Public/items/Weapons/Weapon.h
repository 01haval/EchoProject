// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/item.h"
#include "Weapon.generated.h"


class USoundBase;
class UBoxComponent;

UCLASS()
class CASTLE_API AWeapon : public Aitem
{
	GENERATED_BODY()
	
public:
	 AWeapon();
	void Equip(USceneComponent* Inparent,FName InSocketName,AActor* NewOwner,APawn* NewInstigator);

	void AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay() override;

	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;



	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);
private:
	UPROPERTY(EditAnywhere,Category ="Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere,Category ="Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

public:
	FORCEINLINE	UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
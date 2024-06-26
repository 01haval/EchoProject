// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "EchoCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

class UInputAction;
class UInputMappingContext;

class UGroomComponent;

class Aitem;
class ASoul;
class ATreasure;

class UAnimMontage;

class UEchoOverLay;

UCLASS()
class CASTLE_API AEchoCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEchoCharacter();
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(Aitem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/**
	* input functions 
	*/
	void MoveForward(float Value);
	void Movement(const FInputActionValue& Value);
	void EchoLook(const FInputActionValue& Value);
	void Turn(float value);
	void LookUp(float value);
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Movement;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_EchoLook;
	UPROPERTY(EditAnywhere,Category=Input)
	UInputMappingContext* IMC_EchoCharcter;

	/*
	* Combat
	*/
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;	
	bool CanDisarm();
	bool CanArm();
	void Arm();
	void Disarm();
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die_Implementation() override;
	bool HasEnoughStamina();
	bool IsOccupied();


	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinshEquiping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	bool IsUnoccupied();
	void InitiallizeEchoOverlay();
	void SetHUDHealth();

	/*
	* Character Components
	*/

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,Category = Hair);
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair);
	UGroomComponent* Eyebrows;
	
	UPROPERTY(VisibleInstanceOnly)
	Aitem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* EquipMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	UEchoOverLay* EchoOverlay;


public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

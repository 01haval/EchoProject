// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "EchoCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

class UInputAction;
class UInputMappingContext;

class UGroomComponent;

class Aitem;

class UAnimMontage;

UCLASS()
class CASTLE_API AEchoCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEchoCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Movement;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_EchoLook;
	UPROPERTY(EditAnywhere,Category=Input)
	UInputMappingContext* IMC_EchoCharcter;

	/*
	* Montage Functions
	*/

	virtual void AttackEnd() override;
	virtual bool CanAttack() override;


	bool CanDisarm();
	bool CanArm();
	void PlayEquipMontage(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinshEquiping();

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess= "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

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

public:
	FORCEINLINE void SetOverlappingItem(Aitem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};

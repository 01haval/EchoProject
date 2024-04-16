// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EchoCharacter.h"
#include "EnhancedInputSubsystems.h"
//enhanced input heder files
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
// camera and spring arm
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//for movement 
#include "GameFramework/CharacterMovementComponent.h"
//for hair and eyebrows
#include "GroomComponent.h"
//for the equip an item 
#include "items/item.h"
#include "items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
//for box component collision
#include "Components/BoxComponent.h"



// Sets default values
AEchoCharacter::AEchoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	
}

// Called when the game starts or when spawned
void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_EchoCharcter, 0);
		}
	}

	Tags.Add(FName("EchoCharacter"));
	
}

void AEchoCharacter::MoveForward(float Value)
{
	if ((Value != 0) && (Controller))
	{
		FVector ForWordLocation = GetActorForwardVector();
		AddMovementInput(ForWordLocation, Value);
	}

}

void AEchoCharacter::Movement(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}
	const FVector2D MoveValue =Value.Get<FVector2D>();

	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawController(0.f, ControllerRotation.Yaw, 0.f);
	
	const FVector DirectionX = FRotationMatrix(YawController).GetUnitAxis(EAxis::X);
	const FVector DirectionY = FRotationMatrix(YawController).GetUnitAxis(EAxis::Y);


	AddMovementInput(DirectionX, MoveValue.Y);
	AddMovementInput(DirectionY, MoveValue.X);

	/*FVector Right = GetActorRightVector();
	AddMovementInput(Right, MoveValue.X);*/
}

void AEchoCharacter::EchoLook(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();
	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);
}

void AEchoCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void AEchoCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AEchoCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"),this,this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else {
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void AEchoCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		ActionState = EActionState::EAS_Attacking;
		PlayAttackMontage();
	}
}

bool AEchoCharacter::CanAttack()
{
	return (ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped);
}

bool AEchoCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AEchoCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AEchoCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AEchoCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AEchoCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AEchoCharacter::FinshEquiping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEchoCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}



// Called every frame
void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this,&AEchoCharacter::Movement );
		EnhancedInputComponent->BindAction(IA_EchoLook, ETriggerEvent::Triggered, this, &AEchoCharacter::EchoLook);
	}

	PlayerInputComponent->BindAction(FName("Jump"),IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AEchoCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AEchoCharacter::Attack);


	/*
	* old way of input 
		PlayerInputComponent->BindAxis(FName("MoveForword"), this, &AEchoCharacter::MoveForward);
		PlayerInputComponent->BindAxis(FName("Turn"), this, &AEchoCharacter::Turn);
		PlayerInputComponent->BindAxis(FName("LookUp"), this, &AEchoCharacter::LookUp);
	*/

}



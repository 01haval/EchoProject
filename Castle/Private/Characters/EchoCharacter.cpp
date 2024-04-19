

#include "Characters/EchoCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Components/StaticMeshComponent.h"
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




// Sets default values
AEchoCharacter::AEchoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


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

	Tags.Add(FName("EngageableTarget"));
	
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
		EquipWeapon(OverlappingWeapon);
	}
	else {
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
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

void AEchoCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AEchoCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
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

void AEchoCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AEchoCharacter::AttachWeaponToHand()
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

void AEchoCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AEchoCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
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

void AEchoCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticales(ImpactPoint);
}



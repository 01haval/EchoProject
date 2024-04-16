// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"




// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);


	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0); 
		}
	}


}
/*
* old way of input system.
void ABird::MoveForword(float value)
{
	if((value!=0)&&(Controller!=nullptr))
	{
		FVector Forword = GetActorForwardVector();
		AddMovementInput(Forword, value);

	}

}
*/



/*
* old way of input system
void ABird::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABird::LookUp(float value)
{
	AddControllerPitchInput(value);
}
*/

void ABird::Move(const FInputActionValue& Value)
{
	float DirectionValue = Value.Get<float>();
	if ((DirectionValue != 0) && (Controller != nullptr))
	{
		FVector Forword = GetActorForwardVector();
		AddMovementInput(Forword, DirectionValue);

	}
}

void ABird::LookAction(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &ABird::LookAction);

	}

	//the old way of input system and how we binding MoveForword function to our tow keys wich was w and s 
	//PlayerInputComponent->BindAxis(FName("MoveForword"), this, &ABird::MoveForword);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	//PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);
	
}


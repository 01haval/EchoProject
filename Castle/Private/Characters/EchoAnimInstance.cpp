// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EchoAnimInstance.h"
#include "Characters/EchoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); 

	EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());
	if (EchoCharacter)
	{
		MovementComponent = EchoCharacter->GetCharacterMovement();


	}
	
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MovementComponent)
	{
		 GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		 isFaling = MovementComponent->IsFalling();
		 CharacterState = EchoCharacter->GetCharacterState();
	}

}

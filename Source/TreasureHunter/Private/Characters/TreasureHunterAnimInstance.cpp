// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TreasureHunterAnimInstance.h"

#include "Characters/TreasureHunterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UTreasureHunterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TreasureHunterCharacter = Cast<ATreasureHunterCharacter>(TryGetPawnOwner());
	if(TreasureHunterCharacter != nullptr)
	{
		TreasureHunterMovementComponent = TreasureHunterCharacter->GetCharacterMovement();
	}
}

void UTreasureHunterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(TreasureHunterMovementComponent != nullptr)
	{
		GroundSpeed	= UKismetMathLibrary::VSizeXY(TreasureHunterMovementComponent->Velocity);
		IsFalling = TreasureHunterMovementComponent->IsFalling();
		CharacterState = TreasureHunterCharacter->GetCharacterState();
		ActionState = TreasureHunterCharacter->GetActionState();
		DeathPose = TreasureHunterCharacter->GetDeathPose();
	}
}

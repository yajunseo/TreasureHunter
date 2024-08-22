// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "TreasureHunter/DebugMecros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	const FVector Forward = GetActorForwardVector();
	const FVector ToHit = (FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z) - GetActorLocation()).GetSafeNormal();

	float Theta = FMath::Acos(FVector::DotProduct(Forward, ToHit));
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if(CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	
	if(-45.f <= Theta && Theta < 45.f)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("FromFront"));
		PlayHitReactMontage(FName("FromFront"));
	}
	else if(45.f <= Theta && Theta < 135.f)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("FromRight"));
		PlayHitReactMontage(FName("FromRight"));
	}
	else if(-135.f <= Theta && Theta < -45.f)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("FromLeft"));
		PlayHitReactMontage(FName("FromLeft"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("FromBack"));
		PlayHitReactMontage(FName("FromBack"));
	}
}


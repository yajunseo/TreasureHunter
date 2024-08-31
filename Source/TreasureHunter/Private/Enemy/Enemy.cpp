// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include <string>

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TreasureHunter/DebugMecros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attribute->GetHealthPercent());
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DeathMontage)
	{
		const int32 SectionNum = DeathMontage->GetNumSections();
		if(SectionNum > 0)
		{
			const int32 Selection = FMath::RandRange(1,SectionNum);
			FString SectionString = FString::Printf(TEXT("Death%d"), Selection);
			FName SectionName = FName(*SectionString);

			DeathPos = static_cast<EDeathPose>(Selection);
			
			AnimInstance->Montage_Play(DeathMontage);
			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
		}
	}

	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
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

	if(CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();

		if(DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;

			if(HealthBarWidget)
				HealthBarWidget->SetVisibility(false);
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
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
		PlayHitReactMontage(FName("FromFront"));
	else if(45.f <= Theta && Theta < 135.f)
		PlayHitReactMontage(FName("FromRight"));
	else if(-135.f <= Theta && Theta < -45.f)
		PlayHitReactMontage(FName("FromLeft"));
	else
		PlayHitReactMontage(FName("FromBack"));
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	if(Attribute && Attribute->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	
	else
	{
		Die();
		return;
	}

	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			HitSound,
			ImpactPoint
			);
	}

	if(HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitParticles,
			ImpactPoint);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Attribute && HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
		Attribute->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attribute->GetHealthPercent());
	}

	CombatTarget = EventInstigator->GetPawn();
	
	return DamageAmount;
}


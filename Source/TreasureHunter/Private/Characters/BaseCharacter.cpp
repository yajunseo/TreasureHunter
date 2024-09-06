

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	IHitInterface::GetHit_Implementation(ImpactPoint, Hitter);
	
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
	if(IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	
	else
	{
		Die();
	}
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::Attack()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

bool ABaseCharacter::IsAlive()
{
	return Attribute && Attribute->IsAlive();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			HitSound,
			ImpactPoint
			);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if(HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitParticles,
			ImpactPoint);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if(Attribute)
	{
		Attribute->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if(CombatTarget == nullptr)
		return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	
	const FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal() * WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if(CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}

	return FVector();
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if(SectionNames.Num() <= 0)
		return -1;

	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 SectionIndex = FMath::RandRange(0, MaxSectionIndex);
	
	PlayMontageSection(Montage, SectionNames[SectionIndex]);
	return SectionIndex;
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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

	FName Section("FromBack");
	
	if(-45.f <= Theta && Theta < 45.f)
		Section = FName("FromFront");
	else if(45.f <= Theta && Theta < 135.f)
		Section = FName("FromRight");
	else if(-135.f <= Theta && Theta < -45.f)
		Section = FName("FromLeft");
	else
		Section = FName("FromBack");

	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if(EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Characters/TreasureHunterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::AIMoveTo(const AActor* TargetActor)
{
	if(EnemyController && TargetActor)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(TargetActor);
		MoveRequest.SetAcceptanceRadius(50.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
	}
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;

	for(AActor* target : PatrolTargets)
	{
		if(target != PatrolTarget)
		{
			ValidTargets.AddUnique(target);
		}
	}
			
	const int32 PatrolTargetsNum = ValidTargets.Num();
	if(PatrolTargetsNum > 0)
	{
		const int32 RandomPatrolIndex = FMath::RandRange(0, PatrolTargetsNum - 1);
		PatrolTarget = ValidTargets[RandomPatrolIndex];

		return PatrolTarget;
	}

	return nullptr;
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, AttackMontage->CompositeSections.Num() - 1);
		FName SelectionName = FName();
		switch (Selection)
		{
		case 0:
			SelectionName = FName("Attack1");
			break;
		case 1:
			SelectionName = FName("Attack2");
			break;
		case 2:
			SelectionName = FName("Attack3");
			break;
		default:
			break;;
		}

		AnimInstance->Montage_JumpToSection(SelectionName, AttackMontage);
	}
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = IsInSideAttackRadius()
		&& !IsAttacking()
		&& !IsDead();

	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if(Attribute && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attribute->GetHealthPercent());
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Dead
		|| EnemyState != EEnemyState::EES_Patrolling
		|| !SeenPawn->ActorHasTag(FName("TreasureHunter")))
		return;

	CombatTarget = SeenPawn;
	ClearPatrolTimer();
	ChaseTarget();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attribute->GetHealthPercent());
		HideHealthBar();
	}

	EnemyController = Cast<AAIController>(GetController());
	AIMoveTo(PatrolTarget);

	UWorld* World = GetWorld();
	if(World && WeaponClass)
	{
		EquippedWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
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

	HideHealthBar();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if(Target == nullptr)
		return false;
	
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius ? true : false;
}

void AEnemy::PatrolTimerFinished()
{
	AIMoveTo(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::CombatTargetOutRange()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

bool AEnemy::IsOutSidCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutSideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInSideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = patrolSpeed;
	AIMoveTo(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	AIMoveTo(CombatTarget);
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::CheckCombatTarget()
{
	if(IsOutSidCombatRadius())
	{
		ClearAttackTimer();
		CombatTargetOutRange();

		if(!IsEngaged())
		{
			StartPatrolling();
		}
	}

	else if(IsOutSideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();

		if(!IsEngaged())
		{
			ChaseTarget();
		}
	}
	
	else if(CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if(InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead())
		return;
	
	if(EnemyState != EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();		
	}

	else
	{
		CheckPatrolTarget();	
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();
	
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
	
	if(IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	
	else
	{
		Die();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	Super::Destroyed();
	
	if(EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}	
}


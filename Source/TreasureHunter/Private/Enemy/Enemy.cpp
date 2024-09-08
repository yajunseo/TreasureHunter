// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Characters/TreasureHunterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Soul.h"
#include "Items/Weapons/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

	if(CombatTarget == nullptr)
	{
		return;
	}
	
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = IsInSideAttackRadius()
		&& !IsAttacking()
		&& !IsEngaged() 
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

void AEnemy::AttackEnd()
{
	Super::AttackEnd();

	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Dead
		|| EnemyState != EEnemyState::EES_Patrolling
		|| !SeenPawn->ActorHasTag(FName("EngageableTarget")))
		return;

	CombatTarget = SeenPawn;
	ClearPatrolTimer();
	ChaseTarget();
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if(World && WeaponClass)
	{
		EquippedWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("Enemy"));
	
	EnemyController = Cast<AAIController>(GetController());
	AIMoveTo(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	UE_LOG(LogTemp, Warning, TEXT("1"));
	if(World && Attribute && SoulClass)
	{
		ASoul* SpawnSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("2"));
		if(SpawnSoul)
		{
			UE_LOG(LogTemp, Warning, TEXT("3"));
			SpawnSoul->SetSouls(Attribute->GetSouls());
		}
	}
}

void AEnemy::Die()
{
	Super::Die();
	
	EnemyState = EEnemyState::EES_Dead;

	ClearAttackTimer();
	PlayDeathMontage();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSoul();
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
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if(!IsDead())
	{
		ShowHealthBar();
	}
	ClearPatrolTimer();
	ClearAttackTimer();
	StopAttackMontage();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	
	if(IsInSideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutSideAttackRadius())
	{
		ChaseTarget();
	}
	
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


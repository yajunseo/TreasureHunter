// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterType.h"
#include "Enemy.generated.h"


UCLASS()
class TREASUREHUNTER_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	// <Actor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	// </Actor>
	
	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>
	
protected:
	// <Actor>
	virtual void BeginPlay() override;
	// </Actor>

	// <ABaseCharacter>
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;
	// </ABaseCharacter>
	
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPos;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
private:
	
	// AI Behavior
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void CombatTargetOutRange();
	bool IsOutSidCombatRadius();
	bool IsOutSideAttackRadius();
	bool IsInSideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void StartPatrolling();
	void ChaseTarget();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void AIMoveTo(const AActor* TargetActor);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY()
	class AAIController* EnemyController;

	FTimerHandle PatrolTimer;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation");	// Current Patrol Target
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation");
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation");
	float PatrolWaitMin = 5.f;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation");
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat");
	float patrolSpeed = 130.f;

	UPROPERTY(EditAnywhere, Category = "Combat");
	float ChasingSpeed = 300.f;

	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat");
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Combat");
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;
};

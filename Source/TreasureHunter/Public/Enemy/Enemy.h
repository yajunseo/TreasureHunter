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
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	
protected:
	virtual void BeginPlay() override;

	virtual void Die() override;
	bool InTargetRange(AActor* Target, double Radius);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPos;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	void AIMoveTo(const AActor* TargetActor);
	AActor* ChoosePatrolTarget();

	virtual void Attack() override;
	virtual void PlayAttackMontage() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
private:
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	// Enemy State
	
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	
	//Navigation
	UPROPERTY()
	class AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation");	// Current Patrol Target
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation");
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation");
	float WaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation");
	float WaitMax = 10.f;
	
	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	void CheckCombatTarget();
	void CheckPatrolTarget();

	// AI Behavior
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

	// Combat
	void StartAttackTimer();
	void ClearAttackTimer();
	
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat");
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Combat");
	float AttackMax = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat");
	float patrolSpeed = 130.f;

	UPROPERTY(EditAnywhere, Category = "Combat");
	float ChasingSpeed = 300.f;
};

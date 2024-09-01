// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterType.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"


UCLASS()
class TREASUREHUNTER_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	virtual void BeginPlay() override;

	void Die();
	bool InTargetRange(AActor* Target, double Radius);
	
	// Montage Function
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPos = EDeathPose::EDP_Alive;

	void AIMoveTo(const AActor* TargetActor);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void SetEnemyState(EEnemyState state, AActor* TargetActor);
	
private:
	// Components
	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attribute;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
	
	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	// Enemy State
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	
	UPROPERTY(EditAnywhere, Category = "Stat");
	float patrolSpeed = 130.f;

	UPROPERTY(EditAnywhere, Category = "Stat");
	float ChasingSpeed = 300.f;
	
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
};

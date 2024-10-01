// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss.h"

void ABoss::Attack()
{
	Super::Attack();

	if(CombatTarget == nullptr)
	{
		return;
	}
	
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();

	// 패턴
}

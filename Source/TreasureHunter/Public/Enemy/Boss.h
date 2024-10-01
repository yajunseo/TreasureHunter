// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API ABoss : public AEnemy
{
	GENERATED_BODY()
	
protected:
	virtual void Attack() override;
};

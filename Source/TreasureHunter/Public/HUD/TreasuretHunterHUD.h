// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TreasuretHunterHUD.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API ATreasuretHunterHUD : public AHUD
{
	GENERATED_BODY()

public:
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = TreasureHunter)
	TSubclassOf<class UTreasureHunterOverlay> TreasureHunterClass;

	UPROPERTY()
	UTreasureHunterOverlay* TreasureHunterOverlay;

public:
	FORCEINLINE UTreasureHunterOverlay* GetTreasureHunterOverlay() const {return TreasureHunterOverlay;}
};

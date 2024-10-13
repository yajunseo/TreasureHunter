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
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = TreasureHunter)
	TSubclassOf<class UTreasureHunterOverlay> TreasureHunterClass;

	UPROPERTY(EditDefaultsOnly, Category = TreasureHunter)
	TSubclassOf<class UGameEndBase> DefeatWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = TreasureHunter)
	TSubclassOf<class UGameEndBase> ClearWidgetClass;
	
	UPROPERTY()
	UTreasureHunterOverlay* TreasureHunterOverlay;

	UPROPERTY()
	UGameEndBase* DefeatWidget;

	UPROPERTY()
	UGameEndBase* ClearWidget;

public:
	void ShowTreasureHunterOverlay();
	void HideTreasureHunterOverlay();
	void ShowDefeatWidget(class UAttributeComponent* AttributeComponent);
	void ShowClearWidget(class UAttributeComponent* AttributeComponent);

	FORCEINLINE UTreasureHunterOverlay* GetTreasureHunterOverlay() const {return TreasureHunterOverlay;}
};

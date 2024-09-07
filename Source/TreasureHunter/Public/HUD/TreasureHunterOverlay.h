// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TreasureHunterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API UTreasureHunterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetGoldText(int32 Gold);
	void SetSoulText(int32 Soul);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulText;
};

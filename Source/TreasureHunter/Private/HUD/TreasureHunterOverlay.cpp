// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TreasureHunterOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UTreasureHunterOverlay::SetHealthBarPercent(float Percent)
{
	if(HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UTreasureHunterOverlay::SetStaminaBarPercent(float Percent)
{
	if(StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UTreasureHunterOverlay::SetGoldText(int32 Gold)
{
	const FString GoldString = FString::Printf(TEXT("%d"), Gold);
	const FText GoldFText = FText::FromString(GoldString);
	GoldText->SetText(GoldFText);
}

void UTreasureHunterOverlay::SetSoulText(int32 Soul)
{
	const FString SoulString = FString::Printf(TEXT("%d"), Soul);
	const FText SoulFText = FText::FromString(SoulString);
	SoulText->SetText(SoulFText);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameEndBase.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"

void UGameEndBase::SetGoldText(int32 Gold)
{
	FString GoldString = FString::Printf(TEXT("%d"), Gold);
	FText GoldTxt = FText::FromString(GoldString);
	GoldText->SetText(GoldTxt);
}

void UGameEndBase::SetSoulText(int32 Soul)
{
	FString SoulString = FString::Printf(TEXT("%d"), Soul);
	FText  SoulTxt = FText::FromString(SoulString);
	SoulText->SetText(SoulTxt);
}

void UGameEndBase::SetScoreText(int32 Score)
{
	FString ScoreString = FString::Printf(TEXT("%d"), Score);
	FText  ScoreTxt = FText::FromString(ScoreString);
	ScoreText->SetText(ScoreTxt);
}

void UGameEndBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameEndBase::RestartGame);
	}
}

void UGameEndBase::SetEndUI(int32 Gold, int32 Soul)
{
	int32 Score = Gold * Soul;

	if(Gold == 0 || Soul == 0)
	{
		Score = Gold + Soul;
	}
	
	SetGoldText(Gold);
	SetSoulText(Soul);
	SetScoreText(Score);
}

void UGameEndBase::RestartGame()
{
	ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));

	if(GameMode)
	{
		GameMode->RestartGame();
	}
}

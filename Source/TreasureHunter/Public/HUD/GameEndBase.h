// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndBase.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API UGameEndBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void SetEndUI(int32 Gold, int32 Soul);
	UFUNCTION()
	void RestartGame();

private:
	void SetGoldText(int32 Gold);
	void SetSoulText(int32 Soul);
	void SetScoreText(int32 Score);
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
};

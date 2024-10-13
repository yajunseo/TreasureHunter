// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TreasuretHunterHUD.h"
#include "HUD/TreasureHunterOverlay.h"
#include "HUD/GameEndBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/AttributeComponent.h"

void ATreasuretHunterHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController && TreasureHunterClass)
		{
			TreasureHunterOverlay = CreateWidget<UTreasureHunterOverlay>(PlayerController, TreasureHunterClass);
			TreasureHunterOverlay->AddToViewport();
		}
	}
}

void ATreasuretHunterHUD::ShowTreasureHunterOverlay()
{
	if(TreasureHunterOverlay)
	{
		TreasureHunterOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATreasuretHunterHUD::HideTreasureHunterOverlay()
{
	if(TreasureHunterOverlay)
	{
		TreasureHunterOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATreasuretHunterHUD::ShowDefeatWidget(UAttributeComponent* AttributeComponent)
{
	HideTreasureHunterOverlay();

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController && DefeatWidgetClass)
		{
			DefeatWidget = CreateWidget<UGameEndBase>(PlayerController, DefeatWidgetClass);
			DefeatWidget->AddToViewport();
			DefeatWidget->SetEndUI(AttributeComponent->GetGold(), AttributeComponent->GetSouls());
		}
	}
}

void ATreasuretHunterHUD::ShowClearWidget(UAttributeComponent* AttributeComponent)
{
	HideTreasureHunterOverlay();

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController && ClearWidgetClass)
		{
			ClearWidget = CreateWidget<UGameEndBase>(PlayerController, ClearWidgetClass);
			ClearWidget->AddToViewport();
			ClearWidget->SetEndUI(AttributeComponent->GetGold(), AttributeComponent->GetSouls());
		}
	}
}

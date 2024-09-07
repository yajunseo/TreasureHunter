// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TreasuretHunterHUD.h"
#include "HUD/TreasureHunterOverlay.h"
#include "Blueprint/UserWidget.h"

void ATreasuretHunterHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController && TreasureHunterClass)
		{
			TreasureHunterOverlay =  CreateWidget<UTreasureHunterOverlay>(PlayerController, TreasureHunterClass);
			TreasureHunterOverlay->AddToViewport();
		}
	}
}

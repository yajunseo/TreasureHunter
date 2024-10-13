// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeArena.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"

void AGameModeArena::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if(World)
	{
		ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(World));

		if(GameMode)
		{
			GameMode->LoadGame();
		}
	}

	// test
	PlayBossAppearLevelSequence();
}

void AGameModeArena::CheckClearGame()
{
	UWorld* World = GetWorld();
	
	if(Boss && World)
	{
		if(Boss->IsDead())
		{
			World->GetTimerManager().SetTimer(
				ClearTimeHandle,
				this,
				&AGameModeArena::GameClear,
				3.0f,
				false
			);
		}
	}
}

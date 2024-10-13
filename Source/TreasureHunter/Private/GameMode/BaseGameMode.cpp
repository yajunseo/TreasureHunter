// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseGameMode.h"

#include "LevelSequencePlayer.h"
#include "Characters/TreasureHunterCharacter.h"
#include "Enemy/Enemy.h"
#include "Engine/TargetPoint.h"
#include "HUD/TreasuretHunterHUD.h"
#include "Instance/SaveManager.h"
#include "Instance/SoundManager.h"
#include "Instance/TreasureHunterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UTreasureHunterGameInstance>(UGameplayStatics::GetGameInstance(this));
	if(GameInstance)
	{
		GameInstance->Init();
	}
	
	Player = Cast<ATreasureHunterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	TreasuretHunterHUD = Cast<ATreasuretHunterHUD>(PlayerController->GetHUD());	
	
	PlayLevelBGM();
}

void ABaseGameMode::LoadGame()
{
	if(GameInstance && Player)
	{
		GameInstance->GetSaveManager()->LoadData(Player);
	}
}

void ABaseGameMode::SaveGame()
{
	if(GameInstance && Player)
	{
		GameInstance->GetSaveManager()->SaveData(Player);
	}
}

void ABaseGameMode::PlayLevelBGM()
{
	if(GameInstance && LevelBGM)
	{
		GameInstance->GetSoundManager()->PlayBgm(LevelBGM);
	}
}

void ABaseGameMode::PauseLevelBGM()
{
	if(GameInstance && LevelBGM)
	{
		GameInstance->GetSoundManager()->PauseBGM();
	}
}

void ABaseGameMode::StopLevelBGM()
{
	if(GameInstance && LevelBGM)
	{
		GameInstance->GetSoundManager()->StopBGM();
	}
}

void ABaseGameMode::GameDefeat()
{
	if(TreasuretHunterHUD && PlayerController)
	{
		UGameplayStatics::SetGamePaused(this, true);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;

		TreasuretHunterHUD->ShowDefeatWidget(Player->GetAttribute());
	}
}

void ABaseGameMode::GameClear()
{
	if(TreasuretHunterHUD && PlayerController)
	{
		UGameplayStatics::SetGamePaused(this, true);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		
		TreasuretHunterHUD->ShowClearWidget(Player->GetAttribute());
	}
}

void ABaseGameMode::PlayBossAppearLevelSequence()
{
	if(BossAppearLevelSequence && Boss)
	{
		UWorld* World = GetWorld();
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		PlaybackSettings.bAutoPlay = false;
		ALevelSequenceActor* SequenceActor = nullptr;
		ULevelSequencePlayer *BossAppearPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(World, BossAppearLevelSequence, PlaybackSettings, SequenceActor);

		if(BossAppearPlayer)
		{
			BossAppearPlayer->Play();
			StopLevelBGM();
		}
	}
}

void ABaseGameMode::SpawnBoss()
{
	UWorld* World = GetWorld();
	FindBossSpanwLocation();
	
	if(World && Boss && BossSpawnPoint)
	{
		World->SpawnActor<AEnemy>(Boss, BossSpawnPoint->GetActorLocation(), BossSpawnPoint->GetActorRotation());
		PlayLevelBGM();
	}
}

void ABaseGameMode::RestartGame()
{
	UGameplayStatics::SetGamePaused(this, false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
	
	UGameplayStatics::OpenLevel(this, RestartLevelName);
}

void ABaseGameMode::FindBossSpanwLocation()
{
	UWorld* World = GetWorld();
	if(World)
	{
		TArray<AActor*> FoundTargetPoints;
		UGameplayStatics::GetAllActorsOfClassWithTag(World, ATargetPoint::StaticClass(), BossTagName, FoundTargetPoints);

		if(FoundTargetPoints.Num() > 0)
		{
			BossSpawnPoint = Cast<ATargetPoint>(FoundTargetPoints[0]);
		}
	}
}

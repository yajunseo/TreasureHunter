// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseGameMode.h"

#include "LevelSequencePlayer.h"
#include "Characters/TreasureHunterCharacter.h"
#include "Enemy/Enemy.h"
#include "Engine/TargetPoint.h"
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
		UE_LOG(LogTemp, Warning, TEXT("LevelBGM"));
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

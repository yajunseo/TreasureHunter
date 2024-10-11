// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseGameMode.h"

#include "LevelSequencePlayer.h"
#include "Enemy/Enemy.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
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

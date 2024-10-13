// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/TriggerBoxMove.h"
#include "GameMode/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"

void ATriggerBoxMove::BeginPlay()
{
	Super::BeginPlay();
}

void ATriggerBoxMove::TriggerAction()
{
	Super::TriggerAction();

	SavePlayerData();
}

void ATriggerBoxMove::SavePlayerData()
{
	UWorld* World = GetWorld();

	if(World)
	{
		ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(World));

		if(GameMode)
		{
			GameMode->SaveGame();
		}
	}
	
	ChangeLevel(this, MoveLevelName);
}

void ATriggerBoxMove::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnOverlapBegin(OverlappedActor, OtherActor);
}

void ATriggerBoxMove::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnOverlapEnd(OverlappedActor, OtherActor);
}

void ATriggerBoxMove::ChangeLevel(UObject* WorldContextObject, const FName LevelName)
{
	if(WorldContextObject)
	{
		UGameplayStatics::OpenLevel(WorldContextObject, LevelName);
	}
}

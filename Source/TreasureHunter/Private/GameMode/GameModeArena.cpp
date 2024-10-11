// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeArena.h"

#include "Characters/TreasureHunterCharacter.h"
#include "Instance/SaveInstance.h"
#include "Kismet/GameplayStatics.h"

void AGameModeArena::BeginPlay()
{
	Super::BeginPlay();

	USaveInstance* SaveInstance = Cast<USaveInstance>(GetGameInstance());
	ATreasureHunterCharacter* Player = Cast<ATreasureHunterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if(SaveInstance && Player)
	{
		SaveInstance->LoadData(Player);
	}

	// test
	PlayBossAppearLevelSequence();
}

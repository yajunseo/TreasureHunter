// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/TreasureHunterGameInstance.h"

#include "Instance/SaveManager.h"
#include "Instance/SoundManager.h"

void UTreasureHunterGameInstance::Init()
{
	Super::Init();

	SoundManager = NewObject<USoundManager>();
	SoundManager->Initialize(this);

	SaveManager = NewObject<USaveManager>();
}

USoundManager* UTreasureHunterGameInstance::GetSoundManager()
{
	return SoundManager;
}

USaveManager* UTreasureHunterGameInstance::GetSaveManager()
{
	return SaveManager;
}

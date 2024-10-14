// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/TreasureHunterGameInstance.h"

#include "Instance/SaveManager.h"
#include "Instance/SoundManager.h"
#include "GameFramework/GameUserSettings.h"


void UTreasureHunterGameInstance::Init()
{
	Super::Init();

	// 게임 사용자 설정 가져오기
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (Settings)
	{
		// 해상도 및 창 모드 설정
		Settings->SetScreenResolution(FIntPoint(1280, 720));
		Settings->SetFullscreenMode(EWindowMode::Windowed);  // Windowed 모드 설정
		Settings->ApplySettings(false);  // 설정 적용
	}
	
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

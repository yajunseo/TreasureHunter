// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/SoundManager.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void USoundManager::Initialize(UObject* WorldContextObject)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PlayerController)
	{
		AudioComponent = NewObject<UAudioComponent>(PlayerController);
		AudioComponent->RegisterComponent();
	}
}

void USoundManager::PlayBgm(USoundCue* BGM, float FadeInTime)
{
	if (AudioComponent && BGM)
	{
		AudioComponent->SetSound(BGM);
		AudioComponent->FadeIn(FadeInTime);
	}
}

void USoundManager::PauseBGM()
{
	if (AudioComponent)
	{
		AudioComponent->SetPaused(true);
	}
}

void USoundManager::ResumeBGM()
{
	if (AudioComponent)
	{
		AudioComponent->SetPaused(false);
	}
}

void USoundManager::StopBGM(float FadeOutTime)
{
	if (AudioComponent)
	{
		AudioComponent->FadeOut(FadeOutTime, 0.0f);
	}
}

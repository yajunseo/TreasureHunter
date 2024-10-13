// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API USoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBgm(class USoundCue* BGM, float FadeInTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PauseBGM();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void ResumeBGM();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBGM(float FadeOutTime = 1.0f);

private:
	UPROPERTY()
	class UAudioComponent* AudioComponent;
};

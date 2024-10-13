// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TreasureHunterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API UTreasureHunterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Sound")
	class USoundManager* GetSoundManager();

	UFUNCTION(BlueprintCallable, Category = "Save")
	class USaveManager* GetSaveManager();

private:
	UPROPERTY()
	class USoundManager* SoundManager;

	UPROPERTY()
	class USaveManager* SaveManager;
};

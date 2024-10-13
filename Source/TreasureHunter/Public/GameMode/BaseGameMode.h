// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Save
	UFUNCTION()
	virtual void LoadGame();

	UFUNCTION()
	virtual void SaveGame();

	// BGN
	UFUNCTION()
	virtual void PlayLevelBGM();

	UFUNCTION()
	virtual void PauseLevelBGM();

	UFUNCTION()
	virtual void StopLevelBGM();
	
protected:
	// Appear
	UFUNCTION()
	virtual void PlayBossAppearLevelSequence();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnBoss();
	
private:
	UPROPERTY()
	class UTreasureHunterGameInstance* GameInstance;
	
	UPROPERTY()
	class ATreasureHunterCharacter* Player;

	// BGM
	UPROPERTY(EditAnywhere, Category = "BGM")
	class USoundCue* LevelBGM;
	
	// Boss
	UPROPERTY(EditAnywhere, Category = "Boss")
	class ULevelSequence* BossAppearLevelSequence;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<class AEnemy> Boss;

	UPROPERTY()
	class ATargetPoint* BossSpawnPoint;

	const FName BossTagName = "Boss";

	void FindBossSpanwLocation();
	//
};

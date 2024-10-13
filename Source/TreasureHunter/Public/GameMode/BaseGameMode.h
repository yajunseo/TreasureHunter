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

	// GameEnd
	UFUNCTION()
	void GameDefeat();

	UFUNCTION()
	void GameClear();

	UFUNCTION()
	virtual void CheckClearGame();
	
	// Restart
	UFUNCTION()
	void RestartGame();
	
protected:
	// Appear
	UFUNCTION()
	virtual void PlayBossAppearLevelSequence();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnBoss();

	UPROPERTY()
	class AEnemy* Boss;
	
private:
	UPROPERTY()
	class UTreasureHunterGameInstance* GameInstance;
	
	UPROPERTY()
	class ATreasureHunterCharacter* Player;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	class ATreasuretHunterHUD* TreasuretHunterHUD;

	// BGM
	UPROPERTY(EditAnywhere, Category = "BGM")
	class USoundCue* LevelBGM;
	
	// Boss
	UPROPERTY(EditAnywhere, Category = "Boss")
	class ULevelSequence* BossAppearLevelSequence;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<class AEnemy> BossClass;
	
	UPROPERTY()
	class ATargetPoint* BossSpawnPoint;

	const FName BossTagName = "Boss";

	void FindBossSpanwLocation();
	
	//Restart
	const FName RestartLevelName = "Prison";
};

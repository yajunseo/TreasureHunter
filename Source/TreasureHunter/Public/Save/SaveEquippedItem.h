// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/TreasureHunterCharacter.h"
#include "GameFramework/SaveGame.h"
#include "SaveEquippedItem.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API USaveEquippedItem : public USaveGame
{
	GENERATED_BODY()

	USaveEquippedItem();

public:
	UFUNCTION(BlueprintCallable, Category = "Equipped")
	void SaveEquippedItem(struct FEquippedItem& EquippedItem);

	UFUNCTION(BlueprintCallable, Category = "Equipped")
	void LoadEquippedItem(class ATreasureHunterCharacter* Player);

private:
	UPROPERTY()
	FEquippedItem PlayerEquippedItem;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveData(class ATreasureHunterCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadData(class ATreasureHunterCharacter* Player);

private:
	void SavePlayerAttributes(class UAttributeComponent* AttributeComponent);
	void LoadPlayerAttributes(class UAttributeComponent* AttributeComponent);

	void SavePlayerEquippedItem(struct FEquippedItem& EquippedItem);
	void LoadPlayerEquippedItem(class ATreasureHunterCharacter* Player);

	const FString SaveSlotNameAttribute = TEXT("PlayerAttribute");
	const FString SaveSlotNameEquippedItem = TEXT("PlayerEquippedItem");

	UPROPERTY(EditAnywhere, Category = "Save")
	int32 UserIndex = 0;
};

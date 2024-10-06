// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveInstance.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API USaveInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveData(class ATreasureHunterCharacter* Player);
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadData(class ATreasureHunterCharacter* Player);
	
	void SavePlayerAttributes(class UAttributeComponent* AttributeComponent);
	void LoadPlayerAttributes(class UAttributeComponent* AttributeComponent);
	void SavePlayerEquippedItem(struct FEquippedItem& EquippedItem);
	void LoadPlayerEquippedItem(class ATreasureHunterCharacter* Player);

private:
	const FString SaveSlotNameAttribute = TEXT("PlayerAttribute");
	const FString SaveSlotNameEquippedItem = TEXT("PlayerEquippedItem");
	
	UPROPERTY(EditAnywhere, Category = "Save")
	int32 UserIndex = 0;
};

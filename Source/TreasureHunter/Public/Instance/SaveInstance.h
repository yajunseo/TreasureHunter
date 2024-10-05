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
	void SavePlayerAttributes(class UAttributeComponent* AttributeComponent);
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadPlayerAttributes(class UAttributeComponent* AttributeComponent);

private:
	const FString SaveSlotName = TEXT("PlayerSaveSlot");
	
	UPROPERTY(EditAnywhere, Category = "Save")
	int32 UserIndex = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveAttribute.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API USaveAttribute : public USaveGame
{
	GENERATED_BODY()

public:
	USaveAttribute();

	// Player's attributes to be saved
	UPROPERTY(VisibleAnywhere, Category = "Player Attributes")
	float SavedHealth;

	UPROPERTY(VisibleAnywhere, Category = "Player Attributes")
	float SavedMaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Player Attributes")
	float SavedStamina;

	UPROPERTY(VisibleAnywhere, Category = "Player Attributes")
	float SavedMaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Player Attributes")
	float SavedGold;

	UPROPERTY(VisibleAnywhere, Category = "Player Attributes")
	float SavedSouls;
	
	void SaveAttributesFromComponent(class UAttributeComponent* AttributeComponent);
	
	void LoadAttributesToComponent(class UAttributeComponent* AttributeComponent);
};

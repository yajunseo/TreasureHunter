// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/SaveManager.h"
#include "Characters/TreasureHunterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Save/SaveAttribute.h"
#include "Save/SaveEquippedItem.h"

void USaveManager::SaveData(ATreasureHunterCharacter* Player)
{
	SavePlayerAttributes(Player->GetAttribute());
	SavePlayerEquippedItem(Player->GetPlayerEquippedItem());
}

void USaveManager::LoadData(ATreasureHunterCharacter* Player)
{
	LoadPlayerAttributes(Player->GetAttribute());
	LoadPlayerEquippedItem(Player);
}

void USaveManager::SavePlayerAttributes(UAttributeComponent* AttributeComponent)
{
	USaveAttribute* SaveAttribute = Cast<USaveAttribute>(UGameplayStatics::CreateSaveGameObject(USaveAttribute::StaticClass()));

	SaveAttribute->SaveAttributesFromComponent(AttributeComponent);
	
	bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveAttribute, SaveSlotNameAttribute, UserIndex);
	
	if (bSaveSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Player attributes saved successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save player attributes."));
	}
}

void USaveManager::LoadPlayerAttributes(UAttributeComponent* AttributeComponent)
{
	USaveAttribute* LoaddedGame = Cast<USaveAttribute>(UGameplayStatics::LoadGameFromSlot(SaveSlotNameAttribute, UserIndex));

	if(LoaddedGame)
	{
		LoaddedGame->LoadAttributesToComponent(AttributeComponent);
		UE_LOG(LogTemp, Log, TEXT("Player attributes loaded successfully."));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load player attributes."));
	}
}

void USaveManager::SavePlayerEquippedItem(struct FEquippedItem& EquippedItem)
{
	USaveEquippedItem* SaveItem = Cast<USaveEquippedItem>(UGameplayStatics::CreateSaveGameObject(USaveEquippedItem::StaticClass()));
	SaveItem->SaveEquippedItem(EquippedItem);
	
	bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveItem, SaveSlotNameEquippedItem , UserIndex);

	if (bSaveSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Player equipped item saved successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save player equipped item."));
	}
}

void USaveManager::LoadPlayerEquippedItem(class ATreasureHunterCharacter* Player)
{
	USaveEquippedItem* LoadItem = Cast<USaveEquippedItem>(UGameplayStatics::LoadGameFromSlot(SaveSlotNameEquippedItem, UserIndex));

	if(LoadItem)
	{
		LoadItem->LoadEquippedItem(Player);
		UE_LOG(LogTemp, Log, TEXT("Player equipped item loaded successfully."));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load player equipped item."));
	}
}

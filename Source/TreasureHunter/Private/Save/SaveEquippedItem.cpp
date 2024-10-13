// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/SaveEquippedItem.h"

#include "Characters/TreasureHunterCharacter.h"

USaveEquippedItem::USaveEquippedItem()
{
}

void USaveEquippedItem::SaveEquippedItem(struct FEquippedItem& EquippedItem)
{
	PlayerEquippedItem = EquippedItem;
}

void USaveEquippedItem::LoadEquippedItem(class ATreasureHunterCharacter* Player)
{
	if(Player)
	{
		Player->SetPlayerEquippedItem(PlayerEquippedItem);
	}
}

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
	UE_LOG(LogTemp, Warning, TEXT("111"));
	int32 a = 3;

	if(Player)
	{
		if(PlayerEquippedItem.WeaponName.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("333"));
		}
		
		Player->SetPlayerEquippedItem(PlayerEquippedItem);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("222"));
	}
}

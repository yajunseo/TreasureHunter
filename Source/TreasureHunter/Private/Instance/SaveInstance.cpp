// Fill out your copyright notice in the Description page of Project Settings.

#include "Instance/SaveInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Save/SaveAttribute.h"

void USaveInstance::SavePlayerAttributes(UAttributeComponent* AttributeComponent)
{
	USaveAttribute* SaveAttribute = Cast<USaveAttribute>(UGameplayStatics::CreateSaveGameObject(USaveAttribute::StaticClass()));

	SaveAttribute->SaveAttributesFromComponent(AttributeComponent);
	
	bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveAttribute, SaveSlotName, UserIndex);
	
	if (bSaveSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Player attributes saved successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save player attributes."));
	}
}

void USaveInstance::LoadPlayerAttributes(UAttributeComponent* AttributeComponent)
{
	USaveAttribute* LoaddedGame = Cast<USaveAttribute>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

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

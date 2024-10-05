// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/SaveAttribute.h"
#include "Components/AttributeComponent.h"

USaveAttribute::USaveAttribute()
{
	SavedHealth = 0.f;
	SavedMaxHealth = 0.f;
	SavedStamina = 0.f;
	SavedMaxStamina = 0.f;
	SavedGold = 0.f;
	SavedSouls = 0.f;
}

void USaveAttribute::SaveAttributesFromComponent(UAttributeComponent* AttributeComponent)
{
	if (AttributeComponent)
	{
		SavedHealth = AttributeComponent->GetHealthPercent() * AttributeComponent->GetMaxHealth();  // Current health value
		SavedMaxHealth = AttributeComponent->GetMaxHealth();
		SavedStamina = AttributeComponent->GetStaminaPercent() * AttributeComponent->GetMaxStamina(); // Current stamina value
		SavedMaxStamina = AttributeComponent->GetMaxStamina();
		SavedGold = AttributeComponent->GetGold();
		SavedSouls = AttributeComponent->GetSouls();
	}
}

void USaveAttribute::LoadAttributesToComponent(UAttributeComponent* AttributeComponent)
{
	if (AttributeComponent)
	{
		AttributeComponent->SetHealth(SavedHealth);
		AttributeComponent->SetMaxHealth(SavedMaxHealth);
		AttributeComponent->SetStamina(SavedStamina);
		AttributeComponent->SetMaxStamina(SavedMaxStamina);
		AttributeComponent->AddGold(SavedGold);
		AttributeComponent->AddSouls(SavedSouls);
	}
}

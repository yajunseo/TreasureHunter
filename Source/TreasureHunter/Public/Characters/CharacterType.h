﻿#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UNEQUIPPED UMETA(DisplayName = "Unequipped"),
	ECS_EQUIPPED_ONE_HAND_WEAPON UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EQUIPPED_TWO_HAND_WEAPON UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6"),
};
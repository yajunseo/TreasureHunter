#pragma once


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
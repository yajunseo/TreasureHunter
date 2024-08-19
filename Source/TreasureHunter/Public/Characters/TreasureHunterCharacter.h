// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Character.h"
#include "Items/Item.h"
#include "CharacterType.h"
#include "TreasureHunterCharacter.generated.h"

UCLASS()
class TREASUREHUNTER_API ATreasureHunterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATreasureHunterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* TreasureHunterContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere)
	UInputAction* EquipAction;
	
	UPROPERTY(EditAnywhere)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere)
	UInputAction* DodgeAction;
	
	// Callbacks for input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	void Attack();
	void Dodge();

	// Montage Function
	void PlayAttackMontage();
	void PlayEquipMontage(FName SectionName);
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void DisArm();
	UFUNCTION(BlueprintCallable)
	void Arm();
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
private:
	ECharacterState CharacterState = ECharacterState::ECS_UNEQUIPPED;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AWeapon* EquippedWeapon;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* EquipMontage;
	
	const FName RIGHT_HAND_SOCKET = TEXT("RightHandSocket");
	const FName SPINE_SOCKET = TEXT("SpineSocket");
	
public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Items/Item.h"
#include "CharacterType.h"
#include "Interfaces/PickUpInterface.h"
#include "TreasureHunterCharacter.generated.h"

USTRUCT(BlueprintType)
struct FEquippedItem
{
	GENERATED_BODY()

	FEquippedItem(){};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName;
};

UCLASS()
class TREASUREHUNTER_API ATreasureHunterCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	ATreasureHunterCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetHUDHealth();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Jump() override;
	
	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>

	// <IPickUpInterface>
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	// </IPickUpInterface>
	
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
	virtual void Attack() override;
	void Dodge();

	// Item
	void SpawnWeapon(AWeapon* Weapon);
	
	// Montage Function
	void PlayEquipMontage(FName SectionName);

	virtual void Die_Implementation() override;
	
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void DodgeEnd() override;
	
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void DisArm();
	UFUNCTION(BlueprintCallable)
	void Arm();
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	
private:
	void InitializeTreasureHunterOverlay();
	
	ECharacterState CharacterState = ECharacterState::ECS_UNEQUIPPED;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	FEquippedItem PlayerEquippedItem;
	
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

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* EquipMontage;
	
	const FName RIGHT_HAND_SOCKET = TEXT("RightHandSocket");
	const FName SPINE_SOCKET = TEXT("SpineSocket");

	UPROPERTY()
	class UTreasureHunterOverlay* TreasureHunterOverlay;
	
public:
	void SetPlayerEquippedItem(FEquippedItem& EquippedItem);
	FEquippedItem& GetPlayerEquippedItem();
	
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
};

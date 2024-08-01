// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Character.h"
#include "Items/Item.h"
#include "TreasureHunterCharacter.generated.h"

UCLASS()
class TREASUREHUNTER_API ATreasureHunterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATreasureHunterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	
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
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	void Attack();
	void Dodge();
	
private:
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

	const FName RIGHT_HAND_SOCKET = TEXT("RightHandSocket");
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TREASUREHUNTER_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 15.f;
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Souls;
	
public:
	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	void UseStamina(float StaminaCost);
	float GetStaminaPercent();
	bool IsAlive();
	void AddGold(int32 AddGold);
	void AddSouls(int32 AddSouls);
	void RegenStamina(float DeltaTime);
	FORCEINLINE int32 GetGold() const { return Gold;}
	FORCEINLINE int32 GetSouls() const { return Souls;}
	FORCEINLINE int32 GetStamina() const { return Stamina;}
	FORCEINLINE int32 GetDodgeCost() const { return DodgeCost;}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	TArray<AActor *> IgnoreActors;
	
protected:
	virtual void BeginPlay() override;
	bool CheckHitAndAttackActorIsEnemy(AActor* OtherActor);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	class USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Property")
	class UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	float Damage = 20.f;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const {return WeaponBox;}
};

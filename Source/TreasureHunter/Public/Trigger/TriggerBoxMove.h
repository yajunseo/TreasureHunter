// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/TriggerBoxBase.h"
#include "TriggerBoxMove.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API ATriggerBoxMove : public ATriggerBoxBase
{
	GENERATED_BODY()

virtual void BeginPlay() override;
	
protected:
	virtual void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor) override;
	virtual void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) override;

private:
    void ChangeLevel(UObject* WorldContextObject, const FName LevelName);
	
	UPROPERTY(EditAnywhere, Category = "Move")
	FName MoveLevelName;
};

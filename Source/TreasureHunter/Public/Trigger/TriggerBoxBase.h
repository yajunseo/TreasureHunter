// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerBoxBase.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API ATriggerBoxBase : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATriggerBoxBase();

	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	virtual void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
	virtual void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, Category = "Widget")
	class UWidgetComponent* TriggerWidget;
};
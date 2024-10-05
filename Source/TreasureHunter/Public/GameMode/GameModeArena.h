// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/BaseGameMode.h"
#include "GameModeArena.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTER_API AGameModeArena : public ABaseGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};

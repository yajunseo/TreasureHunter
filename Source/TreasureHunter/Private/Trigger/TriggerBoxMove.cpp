// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/TriggerBoxMove.h"

#include "Characters/TreasureHunterCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Instance/SaveInstance.h"

void ATriggerBoxMove::BeginPlay()
{
	Super::BeginPlay();
}

void ATriggerBoxMove::TriggerAction()
{
	Super::TriggerAction();

	SavePlayerData();
}

void ATriggerBoxMove::SavePlayerData()
{
	USaveInstance* SaveInstance = Cast<USaveInstance>(GetGameInstance());
	ATreasureHunterCharacter* Player = Cast<ATreasureHunterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	SaveInstance->SaveData(Player);
	
	ChangeLevel(this, MoveLevelName);
}

void ATriggerBoxMove::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnOverlapBegin(OverlappedActor, OtherActor);
}

void ATriggerBoxMove::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnOverlapEnd(OverlappedActor, OtherActor);
}

void ATriggerBoxMove::ChangeLevel(UObject* WorldContextObject, const FName LevelName)
{
	if(WorldContextObject)
	{
		UGameplayStatics::OpenLevel(WorldContextObject, LevelName);
	}
}

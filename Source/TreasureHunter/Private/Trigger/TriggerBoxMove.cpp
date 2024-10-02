// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/TriggerBoxMove.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

void ATriggerBoxMove::BeginPlay()
{
	Super::BeginPlay();
}

void ATriggerBoxMove::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnOverlapBegin(OverlappedActor, OtherActor);
	
	ChangeLevel(this, MoveLevelName);
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

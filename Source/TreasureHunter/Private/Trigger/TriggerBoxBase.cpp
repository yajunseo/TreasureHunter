// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/TriggerBoxBase.h"

#include "Characters/TreasureHunterCharacter.h"
#include "Components/ShapeComponent.h"
#include "Components/WidgetComponent.h"


ATriggerBoxBase::ATriggerBoxBase()
{
	TriggerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));

	TriggerWidget->SetupAttachment(RootComponent);
}

void ATriggerBoxBase::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxBase::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerBoxBase::OnOverlapEnd);
	
	if(TriggerWidget && TriggerWidget->GetWidget())
	{
		TriggerWidget->SetHiddenInGame(false);
	}
}

void ATriggerBoxBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(TriggerWidget && TriggerWidget->GetWidget())
	{
		TriggerWidget->SetVisibility(true);
	}
}

void ATriggerBoxBase::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if(TriggerWidget && TriggerWidget->GetWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("falseFalse"));
	}
}

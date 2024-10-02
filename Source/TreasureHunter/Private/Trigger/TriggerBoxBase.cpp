// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/TriggerBoxBase.h"

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

	UE_LOG(LogTemp, Warning, TEXT("start"));

	if(TriggerWidget && TriggerWidget->GetWidget())
	{
		//TriggerWidget->SetHiddenInGame(false);
		UE_LOG(LogTemp, Warning, TEXT("hide false"));
	}
}

void ATriggerBoxBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(TriggerWidget && TriggerWidget->GetWidget())
	{
		//TriggerWidget->SetVisibility(true);
		UE_LOG(LogTemp, Warning, TEXT("true"));
	}

	UE_LOG(LogTemp, Warning, TEXT("trueTRUE"));
}

void ATriggerBoxBase::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if(TriggerWidget && TriggerWidget->GetWidget())
	{
		//TriggerWidget->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("falseFalse"));
	}
}

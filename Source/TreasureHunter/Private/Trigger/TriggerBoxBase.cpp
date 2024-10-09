// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/TriggerBoxBase.h"

#include "Characters/TreasureHunterCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"


ATriggerBoxBase::ATriggerBoxBase()
{
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render Component"));

	TextRender->SetupAttachment(RootComponent);
}

void ATriggerBoxBase::BeginPlay()
{
	Super::BeginPlay();

	GetSpriteComponent()->SetHiddenInGame(true);
	IsTrigger = false;
	
	OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxBase::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerBoxBase::OnOverlapEnd);
	
	if(TextRender)
	{
		TextRender->SetVisibility(false);
	}
}

void ATriggerBoxBase::TriggerAction()
{
}

void ATriggerBoxBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	IsTrigger = true;
	
	if(TextRender)
	{
		TextRender->SetVisibility(true);
	}
}

void ATriggerBoxBase::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	IsTrigger = false;
	
	if(TextRender)
	{
		TextRender->SetVisibility(false);
	}
}

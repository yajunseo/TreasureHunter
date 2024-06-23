// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include  "TreasureHunter/DebugMecros.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	FVector Location = GetActorLocation();
	FVector ForwardLocation = Location + GetActorForwardVector() * 100.f;
	// DRAW_SPHERE(Location);
	// DRAW_LINE(Location, ForwardLocation);
	// DRAW_POINT(ForwardLocation);
	DRAW_VECTOR(Location, ForwardLocation);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(GEngine != nullptr)
    {
	    GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, FString::Format(TEXT("{0}"), {DeltaTime}));
    }
}


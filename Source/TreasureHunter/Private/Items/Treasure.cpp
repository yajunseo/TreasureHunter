// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"

#include "Characters/TreasureHunterCharacter.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if(PickUpInterface)
	{
		PickUpInterface->AddGold(this);
		
		SpawnPickUpSound();
		Destroy();
	}
}

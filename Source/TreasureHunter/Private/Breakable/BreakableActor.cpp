

#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if(bBroken)
		return;
	
	bBroken = true;
	UWorld* World = GetWorld();

	if(World && TreasureClasses.Num() > 0)
	{
		int32 randomIndex = FMath::RandRange(0, TreasureClasses.Num() - 1);
		
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<ATreasure>(TreasureClasses[randomIndex], Location, GetActorRotation());
	}
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::AfterBreak);

	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void ABreakableActor::AfterBreak(const FChaosBreakEvent& BreakEvent)
{
	if(bAfterBrokenMethodCall)
		return;

	bAfterBrokenMethodCall = true;
	
	SetLifeSpan(3.0f);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

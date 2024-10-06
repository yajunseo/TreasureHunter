

#include "Characters/TreasureHunterCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/TreasureHunterOverlay.h"
#include "HUD/TreasuretHunterHUD.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Weapons/Weapon.h"

ATreasureHunterCharacter::ATreasureHunterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");
	
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ATreasureHunterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsAlive() && Attribute && TreasureHunterOverlay)
	{
		Attribute->RegenStamina(DeltaSeconds);
		TreasureHunterOverlay->SetStaminaBarPercent(Attribute->GetStaminaPercent());
	}
}

void ATreasureHunterCharacter::InitializeTreasureHunterOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		ATreasuretHunterHUD* TreasuretHunterHUD = Cast<ATreasuretHunterHUD>(PlayerController->GetHUD());

		if(TreasuretHunterHUD)
		{
			TreasureHunterOverlay = TreasuretHunterHUD->GetTreasureHunterOverlay();

			if(TreasureHunterOverlay && Attribute)
			{
				TreasureHunterOverlay->SetHealthBarPercent(Attribute->GetHealthPercent());
				TreasureHunterOverlay->SetStaminaBarPercent(Attribute->GetStaminaPercent());
				TreasureHunterOverlay->SetGoldText(Attribute->GetGold());
				TreasureHunterOverlay->SetSoulText(Attribute->GetSouls());
			}
		}
	}
}

void ATreasureHunterCharacter::SetPlayerEquippedItem(FEquippedItem& EquippedItem)
{
	PlayerEquippedItem = EquippedItem;

	FString WeaponName = PlayerEquippedItem.WeaponName;
	
	if(!WeaponName.IsEmpty())
	{
		// FString WeaponClassPath = FString::Printf(TEXT("%s.%s_C"), *WeaponPath, *WeaponName);
		// UClass* LoadedClass = StaticLoadClass(AWeapon::StaticClass(), nullptr, *WeaponClassPath);
		//
		// if (LoadedClass && LoadedClass->IsChildOf(AWeapon::StaticClass()))
		// {
		// 	AWeapon* LoadWeapon = Cast<AWeapon>(LoadedClass);
		// 	SpawnWeapon(LoadWeapon);
		// 	UE_LOG(LogTemp, Log, TEXT("Successfully loaded and spawned weapon class: %s"), *WeaponName);
		// }
		// else
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Could not load weapon class for %s. Check class path: %s"), *WeaponName, *WeaponClassPath);
		// }

		if (WeaponName.EndsWith(TEXT("_C")))
		{
			WeaponName = WeaponName.LeftChop(2); // 마지막 2글자('_C')를 제거
		}
		
		FString ClassPath = FString::Printf(TEXT("/Game/Blueprints/Items/Weapons/%s.%s_C"), *WeaponName, *WeaponName);
		UClass* LoadedClass = StaticLoadClass(UObject::StaticClass(), nullptr, *ClassPath);
		UWorld* World = GetWorld();
		
		if (LoadedClass && LoadedClass->IsChildOf(AWeapon::StaticClass()) && World)
		{
			FActorSpawnParameters SpawnParams;
			AWeapon* SpawnEquipWeapon = World->SpawnActor<AWeapon>(LoadedClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			SpawnWeapon(SpawnEquipWeapon);

			UE_LOG(LogTemp, Warning, TEXT("%s spawn success"), *WeaponName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find weapon class for %s"), *WeaponName);
		}
	}
}

FEquippedItem& ATreasureHunterCharacter::GetPlayerEquippedItem()
{
	FString WeaponClassName = EquippedWeapon->GetName();
	int32 Index;
	if (WeaponClassName.FindLastChar('_', Index))
	{
		WeaponClassName = WeaponClassName.Left(Index);
	}
	
	PlayerEquippedItem.WeaponName = WeaponClassName;
	
	return PlayerEquippedItem;
}

// Called when the game starts or when spawned
void ATreasureHunterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(TreasureHunterContext, 0);
	}

	Tags.Add(TEXT("EngageableTarget"));

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	InitializeTreasureHunterOverlay();
}

void ATreasureHunterCharacter::Move(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied)
		return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	
	const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ATreasureHunterCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisValue.Y);
	AddControllerYawInput(LookAxisValue.X);
}

void ATreasureHunterCharacter::Equip()
{
	AWeapon* Weapon = Cast<AWeapon>(OverlappingItem);
	if(Weapon)
	{
		SpawnWeapon(Weapon);
	}
	else
	{
		if(CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_UNEQUIPPED;
			ActionState = EActionState::EAS_EquippingWeapon;
		}

		else if(CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EQUIPPED_ONE_HAND_WEAPON;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void ATreasureHunterCharacter::Attack()
{
	Super::Attack();
	
	if(CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ATreasureHunterCharacter::Dodge()
{
	if(ActionState != EActionState::EAS_Unoccupied)
		return;

	if(TreasureHunterOverlay == nullptr)
		return;

	int32 DodgeStaminaCost = Attribute->GetDodgeCost();
	
	if(!HasEnoughStamina(DodgeStaminaCost))
		return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	Attribute->UseStamina(DodgeStaminaCost);
	TreasureHunterOverlay->SetStaminaBarPercent(Attribute->GetStaminaPercent());
}

void ATreasureHunterCharacter::SpawnWeapon(AWeapon* Weapon)
{
	if(Weapon == nullptr)
		return;
	
	if(EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	
	Weapon->Equip(this->GetMesh(), RIGHT_HAND_SOCKET, this, this);
	CharacterState = ECharacterState::ECS_EQUIPPED_ONE_HAND_WEAPON;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void ATreasureHunterCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ATreasureHunterCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATreasureHunterCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ATreasureHunterCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_UNEQUIPPED;
}

void ATreasureHunterCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}

bool ATreasureHunterCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied
			&& CharacterState != ECharacterState::ECS_UNEQUIPPED;
}

bool ATreasureHunterCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState == ECharacterState::ECS_UNEQUIPPED
		&& EquippedWeapon;
}

void ATreasureHunterCharacter::DisArm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), SPINE_SOCKET);
	}
}

void ATreasureHunterCharacter::Arm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), RIGHT_HAND_SOCKET);
	}
}

void ATreasureHunterCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ATreasureHunterCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ATreasureHunterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ATreasureHunterCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATreasureHunterCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATreasureHunterCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ATreasureHunterCharacter::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATreasureHunterCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ATreasureHunterCharacter::Dodge);
	}
}

void ATreasureHunterCharacter::SetHUDHealth()
{
	if(TreasureHunterOverlay && Attribute)
	{
		TreasureHunterOverlay->SetHealthBarPercent(Attribute->GetHealthPercent());
	}
}

float ATreasureHunterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                           AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	
	return  DamageAmount;
}

void ATreasureHunterCharacter::Jump()
{
	if(ActionState == EActionState::EAS_Unoccupied)
	{
		Super::Jump();
	}
}

void ATreasureHunterCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Attribute && Attribute->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ATreasureHunterCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ATreasureHunterCharacter::AddSouls(ASoul* Soul)
{
	if(Attribute && TreasureHunterOverlay)
	{
		Attribute->AddSouls(Soul->GetSouls());
		TreasureHunterOverlay->SetSoulText(Attribute->GetSouls());
	}
}

void ATreasureHunterCharacter::AddGold(ATreasure* Treasure)
{
	if(Attribute && TreasureHunterOverlay)
	{
		Attribute->AddGold(Treasure->GetGold());
		TreasureHunterOverlay->SetGoldText(Attribute->GetGold());
	}
}





#include "Characters/TreasureHunterCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
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

// Called when the game starts or when spawned
void ATreasureHunterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(TreasureHunterContext, 0);
	}

	Tags.Add(FName("TreasureHunter"));
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
		Weapon->Equip(this->GetMesh(), RIGHT_HAND_SOCKET, this, this);
		CharacterState = ECharacterState::ECS_EQUIPPED_ONE_HAND_WEAPON;
		OverlappingItem = nullptr;
		EquippedWeapon = Weapon;
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
}

void ATreasureHunterCharacter::PlayAttackMontage()
{
	Super::PlayAttackMontage();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, AttackMontage->CompositeSections.Num() - 1);
		FName SelectionName = FName();
		switch (Selection)
		{
		case 0:
			SelectionName = FName("Attack1");
			break;
		case 1:
			SelectionName = FName("Attack2");
			break;
		default:
			break;;
		}

		AnimInstance->Montage_JumpToSection(SelectionName, AttackMontage);
	}
}

void ATreasureHunterCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
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

// Called every frame
void ATreasureHunterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
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



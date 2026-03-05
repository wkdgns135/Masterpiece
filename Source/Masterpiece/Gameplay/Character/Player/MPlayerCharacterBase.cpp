// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/MPlayerCameraComponent.h"
#include "Component/MPlayerInputComponent.h"
#include "Component/MPlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMPlayerCharacterBase::AMPlayerCharacterBase()
{
	PlayerInput = CreateDefaultSubobject<UMPlayerInputComponent>(TEXT("PlayerInputComponent"));
	PlayerInput->SetupAttachment(RootComponent);
	
	PlayerMovement = CreateDefaultSubobject<UMPlayerMovementComponent>(TEXT("PlayerMovementComponent"));
	PlayerMovement->SetupAttachment(RootComponent);
	
	PlayerCamera = CreateDefaultSubobject<UMPlayerCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCamera->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	SpringArm->TargetArmLength = 700.0f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	
}

void AMPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		check(PlayerInput);
		PlayerInput->BindInputActions(EnhancedInputComponent);
	}
}

void AMPlayerCharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
}

void AMPlayerCharacterBase::HandleDeath()
{
}

void AMPlayerCharacterBase::ApplyHealing(float Healing, AActor* Healer)
{
}

void AMPlayerCharacterBase::DoAttackTrace(FName DamageSourceBone)
{
}

void AMPlayerCharacterBase::TriggerPrimaryAttack()
{
	if (PlayerMovement)
	{
		PlayerMovement->FaceCursorDirection();
	}
}

void AMPlayerCharacterBase::TriggerMoveCommand()
{
}

void AMPlayerCharacterBase::TriggerInteraction()
{
}

void AMPlayerCharacterBase::TriggerDodge()
{
}

void AMPlayerCharacterBase::TriggerSkill(const EMSkillSlot SkillSlot)
{
	LastTriggeredSkillSlot = SkillSlot;

	if (PlayerMovement)
	{
		PlayerMovement->FaceCursorDirection();
	}
}

void AMPlayerCharacterBase::TriggerQuickSlot(const EMQuickSlot QuickSlot)
{
	LastTriggeredQuickSlot = QuickSlot;
}

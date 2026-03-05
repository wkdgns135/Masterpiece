// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/MPlayerCameraComponent.h"
#include "Component/MPlayerCombatComponent.h"
#include "Component/MPlayerInputComponent.h"
#include "Component/MPlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMPlayerCharacterBase::AMPlayerCharacterBase()
{
	PlayerInputComponent = CreateDefaultSubobject<UMPlayerInputComponent>(TEXT("PlayerInputComponent"));
	PlayerInputComponent->SetupAttachment(RootComponent);
	
	PlayerMovementComponent = CreateDefaultSubobject<UMPlayerMovementComponent>(TEXT("PlayerMovementComponent"));
	PlayerMovementComponent->SetupAttachment(RootComponent);
	
	PlayerCameraComponent = CreateDefaultSubobject<UMPlayerCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCameraComponent->SetupAttachment(RootComponent);

	PlayerCombatComponent = CreateDefaultSubobject<UMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerCombatComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	SpringArmComponent->TargetArmLength = 700.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	
}

void AMPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent))
	{
		check(this->PlayerInputComponent);
		this->PlayerInputComponent->BindInputActions(EnhancedInputComponent);
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

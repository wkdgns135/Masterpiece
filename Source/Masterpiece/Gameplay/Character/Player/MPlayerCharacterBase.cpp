// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacterBase.h"

#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/MPlayerCameraComponent.h"
#include "Component/MPlayerComponent.h"
#include "Component/MPlayerCombatComponent.h"
#include "Component/MPlayerSkillComponent.h"
#include "Component/MPlayerMovementComponent.h"
#include "Input/MPlayerInputComponent.h"
#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/AbilitySystem/Attribute/MPlayerAttributeSet.h"
#include "Gameplay/PlayerState/MGameplayPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMPlayerCharacterBase::AMPlayerCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMPlayerMovementComponent>(CharacterMovementComponentName))
{
	PlayerCameraComponent = CreateDefaultSubobject<UMPlayerCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCameraComponent->SetupAttachment(RootComponent);

	PlayerComponent = CreateDefaultSubobject<UMPlayerComponent>(TEXT("PlayerComponent"));
	PlayerCombatComponent = CreateDefaultSubobject<UMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	SkillComponent = CreateDefaultSubobject<UMPlayerSkillComponent>(TEXT("SkillComponent"));

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
	SpringArmComponent->SetAbsolute(true, true, true);

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	
}

UMPlayerInputComponent* AMPlayerCharacterBase::GetPlayerInputComponent() const
{
	return Cast<UMPlayerInputComponent>(InputComponent);
}

UMPlayerMovementComponent* AMPlayerCharacterBase::GetPlayerMovementComponent() const
{
	UMPlayerMovementComponent* PlayerMovementComponent = Cast<UMPlayerMovementComponent>(GetCharacterMovement());
	return PlayerMovementComponent;
}

UMPlayerAttributeSet* AMPlayerCharacterBase::GetPlayerAttributeSet() const
{
	if (const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState())
	{
		return GameplayPlayerState->GetPlayerAttributeSet();
	}

	return nullptr;
}

void AMPlayerCharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	UMAbilitySystemComponent* AbilitySystemComponent = GetMAbilitySystemComponent();
	if (!AbilitySystemComponent || Damage <= 0.0f)
	{
		return;
	}
	
}

void AMPlayerCharacterBase::HandleDeath()
{
	GetCharacterMovement()->DisableMovement();
}

void AMPlayerCharacterBase::ApplyHealing(float Healing, AActor* Healer)
{
	UMAbilitySystemComponent* AbilitySystemComponent = GetMAbilitySystemComponent();
	if (!AbilitySystemComponent || Healing <= 0.0f)
	{
		return;
	}
	
}

void AMPlayerCharacterBase::DoAttackTrace(FName DamageSourceBone)
{
}

UAbilitySystemComponent* AMPlayerCharacterBase::ResolveAbilitySystemComponent() const
{
	const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState();
	return GameplayPlayerState ? GameplayPlayerState->GetAbilitySystemComponent() : nullptr;
}

UMCombatAttributeSet* AMPlayerCharacterBase::ResolveCombatAttributeSet() const
{
	const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState();
	return GameplayPlayerState ? GameplayPlayerState->GetCombatAttributeSet() : nullptr;
}

const FBaseStat* AMPlayerCharacterBase::GetDefaultBaseStat() const
{
	return &DefaultPlayerStat.BaseStat;
}

bool AMPlayerCharacterBase::AreStartupAbilitiesGranted() const
{
	const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState();
	return GameplayPlayerState && GameplayPlayerState->AreStartupAbilitiesGranted();
}

void AMPlayerCharacterBase::SetStartupAbilitiesGranted(const bool bGranted)
{
	AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState();
	if (!GameplayPlayerState)
	{
		return;
	}

	GameplayPlayerState->SetStartupAbilitiesGranted(bGranted);
}

AActor* AMPlayerCharacterBase::GetAbilityActorInfoOwner() const
{
	return GetGameplayPlayerState();
}

void AMPlayerCharacterBase::InitializeDefaultAttributes()
{
	Super::InitializeDefaultAttributes();

	UMPlayerAttributeSet* PlayerAttributeSet = GetPlayerAttributeSet();
	if (!PlayerAttributeSet)
	{
		return;
	}

	const float ClampedMana = FMath::Clamp(DefaultPlayerStat.CurrentMana, 0.0f, DefaultPlayerStat.MaxMana);
	PlayerAttributeSet->InitMaxMana(DefaultPlayerStat.MaxMana);
	PlayerAttributeSet->InitMana(ClampedMana);
}

AMGameplayPlayerState* AMPlayerCharacterBase::GetGameplayPlayerState() const
{
	return GetPlayerState<AMGameplayPlayerState>();
}

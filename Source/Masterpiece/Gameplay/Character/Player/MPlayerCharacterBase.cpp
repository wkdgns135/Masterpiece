// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/MPlayerCameraComponent.h"
#include "Component/MPlayerCombatComponent.h"
#include "Component/MPlayerInputComponent.h"
#include "Component/MPlayerMovementComponent.h"
#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Ability/MGameplayAbility.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/AbilitySystem/Attribute/MPlayerAttributeSet.h"
#include "Gameplay/PlayerState/MGameplayPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMPlayerCharacterBase::AMPlayerCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMPlayerMovementComponent>(CharacterMovementComponentName))
{
	PlayerInputComponent = CreateDefaultSubobject<UMPlayerInputComponent>(TEXT("PlayerInputComponent"));
	PlayerInputComponent->SetupAttachment(RootComponent);

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
	SpringArmComponent->SetAbsolute(true, true, true);

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	
}

UMAbilitySystemComponent* AMPlayerCharacterBase::GetMAbilitySystemComponent() const
{
	if (const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState())
	{
		return GameplayPlayerState->GetMAbilitySystemComponent();
	}

	return nullptr;
}

UAbilitySystemComponent* AMPlayerCharacterBase::GetAbilitySystemComponent() const
{
	if (const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState())
	{
		return GameplayPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

UMPlayerMovementComponent* AMPlayerCharacterBase::GetPlayerMovementComponent() const
{
	UMPlayerMovementComponent* PlayerMovementComponent = Cast<UMPlayerMovementComponent>(GetCharacterMovement());
	return PlayerMovementComponent;
}

UMCombatAttributeSet* AMPlayerCharacterBase::GetCombatAttributeSet() const
{
	if (const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState())
	{
		return GameplayPlayerState->GetCombatAttributeSet();
	}

	return nullptr;
}

UMPlayerAttributeSet* AMPlayerCharacterBase::GetPlayerAttributeSet() const
{
	if (const AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState())
	{
		return GameplayPlayerState->GetPlayerAttributeSet();
	}

	return nullptr;
}

void AMPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
}

void AMPlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
}

void AMPlayerCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
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

void AMPlayerCharacterBase::InitializeAbilitySystem()
{
	AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState();
	if (!GameplayPlayerState)
	{
		return;
	}

	UMAbilitySystemComponent* AbilitySystemComponent = GameplayPlayerState->GetMAbilitySystemComponent();
	UMCombatAttributeSet* CombatAttributeSet = GameplayPlayerState->GetCombatAttributeSet();
	UMPlayerAttributeSet* PlayerAttributeSet = GameplayPlayerState->GetPlayerAttributeSet();
	if (!AbilitySystemComponent || !CombatAttributeSet || !PlayerAttributeSet)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(GameplayPlayerState, this);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
		GrantStartupAbilities();
	}
}

void AMPlayerCharacterBase::InitializeDefaultAttributes()
{
	UMAbilitySystemComponent* AbilitySystemComponent = GetMAbilitySystemComponent();
	UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	UMPlayerAttributeSet* PlayerAttributeSet = GetPlayerAttributeSet();
	if (!AbilitySystemComponent || !CombatAttributeSet || !PlayerAttributeSet)
	{
		return;
	}

	const FBaseStat& BaseStat = DefaultPlayerStat.BaseStat;
	const float ClampedHealth = FMath::Clamp(DefaultPlayerStat.BaseStat.CurrentHealth, 0.0f, BaseStat.MaxHealth);
	const float ClampedMana = FMath::Clamp(DefaultPlayerStat.CurrentMana, 0.0f, DefaultPlayerStat.MaxMana);
	CombatAttributeSet->InitMaxHealth(BaseStat.MaxHealth);
	CombatAttributeSet->InitHealth(ClampedHealth);
	CombatAttributeSet->InitMoveSpeed(BaseStat.MoveSpeed);
	CombatAttributeSet->InitAttackPower(BaseStat.AttackPower);
	CombatAttributeSet->InitAttackSpeed(BaseStat.AttackSpeed);
	CombatAttributeSet->InitAttackRange(BaseStat.AttackRange);
	CombatAttributeSet->InitDefense(BaseStat.DefensePower);

	PlayerAttributeSet->InitMaxMana(DefaultPlayerStat.MaxMana);
	PlayerAttributeSet->InitMana(ClampedMana);

	GetCharacterMovement()->MaxWalkSpeed = BaseStat.MoveSpeed;
}

void AMPlayerCharacterBase::GrantStartupAbilities()
{
	AMGameplayPlayerState* GameplayPlayerState = GetGameplayPlayerState();
	UMAbilitySystemComponent* AbilitySystemComponent = GetMAbilitySystemComponent();
	if (!AbilitySystemComponent || !GameplayPlayerState || !HasAuthority() || GameplayPlayerState->AreStartupAbilitiesGranted())
	{
		return;
	}

	for (const TSubclassOf<UMGameplayAbility>& AbilityClass : StartupAbilities)
	{
		if (!AbilityClass)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		if (const UMGameplayAbility* DefaultAbility = AbilityClass->GetDefaultObject<UMGameplayAbility>())
		{
			const FGameplayTag& AbilityTag = DefaultAbility->GetAbilityTag();
			if (AbilityTag.IsValid())
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityTag);
			}
		}

		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	GameplayPlayerState->SetStartupAbilitiesGranted(true);
}

AMGameplayPlayerState* AMPlayerCharacterBase::GetGameplayPlayerState() const
{
	return GetPlayerState<AMGameplayPlayerState>();
}

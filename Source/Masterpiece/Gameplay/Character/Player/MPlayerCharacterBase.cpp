// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/MPlayerCameraComponent.h"
#include "Component/MPlayerCombatComponent.h"
#include "Component/MPlayerInputComponent.h"
#include "Component/MPlayerMovementComponent.h"
#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Attribute/MAttributeSet.h"
#include "Gameplay/AbilitySystem/Ability/MGameplayAbility.h"
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

	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMAttributeSet>(TEXT("AttributeSet"));

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

UAbilitySystemComponent* AMPlayerCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
	InitializeDefaultAttributes();
	GrantStartupAbilities();
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
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AMPlayerCharacterBase::InitializeDefaultAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const FBaseStat& BaseStat = DefaultPlayerStat.BaseStat;
	const float ClampedHealth = FMath::Clamp(DefaultPlayerStat.BaseStat.CurrentHealth, 0.0f, BaseStat.MaxHealth);
	const float ClampedMana = FMath::Clamp(DefaultPlayerStat.CurrentMana, 0.0f, DefaultPlayerStat.MaxMana);
	

	GetCharacterMovement()->MaxWalkSpeed = BaseStat.MoveSpeed;
}

void AMPlayerCharacterBase::GrantStartupAbilities()
{
	if (!AbilitySystemComponent || !HasAuthority())
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
			const FGameplayTag& InputTag = DefaultAbility->GetInputTag();
			if (InputTag.IsValid())
			{
				AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
			}
		}

		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

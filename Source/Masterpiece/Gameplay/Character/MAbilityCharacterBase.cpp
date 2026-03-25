// Fill out your copyright notice in the Description page of Project Settings.

#include "MAbilityCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Ability/MGameplayAbility.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

AMAbilityCharacterBase::AMAbilityCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMAbilitySystemComponent* AMAbilityCharacterBase::GetMAbilitySystemComponent() const
{
	return Cast<UMAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AMAbilityCharacterBase::GetAbilitySystemComponent() const
{
	return ResolveAbilitySystemComponent();
}

UMCombatAttributeSet* AMAbilityCharacterBase::GetCombatAttributeSet() const
{
	return ResolveCombatAttributeSet();
}

void AMAbilityCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
}

void AMAbilityCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
}

void AMAbilityCharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	InitializeAbilitySystem();
}

void AMAbilityCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
}

AActor* AMAbilityCharacterBase::GetAbilityActorInfoOwner() const
{
	return const_cast<AMAbilityCharacterBase*>(this);
}

AActor* AMAbilityCharacterBase::GetAbilityActorInfoAvatar() const
{
	return const_cast<AMAbilityCharacterBase*>(this);
}

void AMAbilityCharacterBase::InitializeAbilitySystem()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	AActor* OwnerActor = GetAbilityActorInfoOwner();
	AActor* AvatarActor = GetAbilityActorInfoAvatar();
	if (!AbilitySystemComponent || !CombatAttributeSet || !OwnerActor || !AvatarActor)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, AvatarActor);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
		GrantStartupAbilities();
	}
}

void AMAbilityCharacterBase::InitializeDefaultAttributes()
{
	UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	const FBaseStat* BaseStat = GetDefaultBaseStat();
	if (!CombatAttributeSet || !BaseStat)
	{
		return;
	}

	const float ClampedHealth = FMath::Clamp(BaseStat->CurrentHealth, 0.0f, BaseStat->MaxHealth);
	CombatAttributeSet->InitMaxHealth(BaseStat->MaxHealth);
	CombatAttributeSet->InitHealth(ClampedHealth);
	CombatAttributeSet->InitMoveSpeed(BaseStat->MoveSpeed);
	CombatAttributeSet->InitAttackPower(BaseStat->AttackPower);
	CombatAttributeSet->InitAttackSpeed(BaseStat->AttackSpeed);
	CombatAttributeSet->InitAttackRange(BaseStat->AttackRange);
	CombatAttributeSet->InitDefense(BaseStat->DefensePower);

	GetCharacterMovement()->MaxWalkSpeed = BaseStat->MoveSpeed;
}

void AMAbilityCharacterBase::GrantStartupAbilities()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (!AbilitySystemComponent || !HasAuthority() || AreStartupAbilitiesGranted())
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

	SetStartupAbilitiesGranted(true);
}

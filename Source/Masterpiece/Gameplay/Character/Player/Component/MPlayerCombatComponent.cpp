// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/Interface/MDamageable.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"

UMPlayerCombatComponent::UMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UMPlayerCombatComponent::ExecutePrimaryAttack()
{
	return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Attack_Primary);
}

bool UMPlayerCombatComponent::CanPrimaryAttack(const AActor* TargetActor) const
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	if (!TargetActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		return false;
	}
	
	const float DistanceToTarget = FVector::Dist2D(GetMPlayerCharacter()->GetActorLocation(), TargetActor->GetActorLocation());
	const float AttackRange = GetPlayerAttackRange();
	
	return DistanceToTarget <= AttackRange;
}

void UMPlayerCombatComponent::CancelPendingPrimaryAttack()
{
	CancelPlayerAbilityTag(MGameplayTags::Ability_Attack_Auto);
	CancelPlayerAbilityTag(MGameplayTags::Ability_Movement);
}

float UMPlayerCombatComponent::GetPlayerAttackRange() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	const UMCombatAttributeSet* CombatAttributeSet = PlayerCharacter ? PlayerCharacter->GetCombatAttributeSet() : nullptr;
	return CombatAttributeSet ? CombatAttributeSet->GetAttackRange() : 0.0f;
}

bool UMPlayerCombatComponent::ExecuteDodge()
{
	return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Dodge);
}

bool UMPlayerCombatComponent::ExecuteInteraction()
{
	return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Interaction);
}

bool UMPlayerCombatComponent::ExecuteSkillSlot(const int32 SkillSlotIndex)
{
	switch (SkillSlotIndex)
	{
	case 0:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_Q);
	case 1:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_W);
	case 2:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_E);
	case 3:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_R);
	default:
		return false;
	}
}

bool UMPlayerCombatComponent::TryActivatePlayerAbilityTag(const FGameplayTag& AbilityTag) const
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	const AActor* PlayerActor = GetMPlayerCharacter();
	UAbilitySystemComponent* AbilitySystemComponent = PlayerActor
		? UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerActor)
		: nullptr;
	if (!AbilitySystemComponent)
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);
	return AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer, true);
}

void UMPlayerCombatComponent::CancelPlayerAbilityTag(const FGameplayTag& AbilityTag) const
{
	if (!AbilityTag.IsValid())
	{
		return;
	}

	const AActor* PlayerActor = GetMPlayerCharacter();
	UAbilitySystemComponent* AbilitySystemComponent = PlayerActor
		? UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerActor)
		: nullptr;
	if (!AbilitySystemComponent)
	{
		return;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);
	AbilitySystemComponent->CancelAbilities(&TagContainer);
}

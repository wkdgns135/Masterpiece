// Fill out your copyright notice in the Description page of Project Settings.

#include "MGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Cost/MAbilityCost.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"

UMGameplayAbility::UMGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

const FGameplayTag& UMGameplayAbility::GetInputTag() const
{
	return InputTag;
}

bool UMGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	for (const TObjectPtr<UMAbilityCost>& AdditionalCost : AdditionalCosts)
	{
		if (!IsValid(AdditionalCost))
		{
			continue;
		}

		if (!AdditionalCost->CheckCost(this))
		{
			return false;
		}
	}

	return true;
}

void UMGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	for (const TObjectPtr<UMAbilityCost>& AdditionalCost : AdditionalCosts)
	{
		if (!IsValid(AdditionalCost))
		{
			continue;
		}

		AdditionalCost->ApplyCost(this);
	}
}

UAbilitySystemComponent* UMGameplayAbility::GetOwnerAbilitySystemComponent() const
{
	return GetAbilitySystemComponentFromActorInfo();
}

AActor* UMGameplayAbility::GetOwnerActor() const
{
	return GetOwningActorFromActorInfo();
}

AActor* UMGameplayAbility::GetAvatarActor() const
{
	return GetAvatarActorFromActorInfo();
}

bool UMGameplayAbility::HasEnoughCost() const
{
	if (!CurrentActorInfo || !CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	return CheckCost(CurrentSpecHandle, CurrentActorInfo);
}

bool UMGameplayAbility::IsAbilityOnCooldown() const
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	const FGameplayTagContainer* CooldownTags = GetCooldownTags();
	return CooldownTags && CooldownTags->Num() > 0 && AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags);
}

bool UMGameplayAbility::CommitAbilitySafely()
{
	if (!CurrentActorInfo || !CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	return CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

void UMGameplayAbility::EndAbilityAsSuccess(const bool bReplicateEndAbility)
{
	if (!CurrentActorInfo)
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, false);
}

void UMGameplayAbility::EndAbilityAsCancelled(const bool bReplicateEndAbility)
{
	if (!CurrentActorInfo)
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, true);
}

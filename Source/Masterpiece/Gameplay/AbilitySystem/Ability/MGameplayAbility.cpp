// Fill out your copyright notice in the Description page of Project Settings.

#include "MGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Cost/MAbilityCost.h"

UMGameplayAbility::UMGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

const FGameplayTag& UMGameplayAbility::GetAbilityTag() const
{
	return AbilityTag;
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

void UMGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	ClearGameplayTagCycleWaiters();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

void UMGameplayAbility::WaitForGameplayTagToBeAddedThenRemoved(const FGameplayTag& GameplayTag, TFunction<void()> OnRemoved)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwnerAbilitySystemComponent();
	if (!AbilitySystemComponent || !GameplayTag.IsValid() || !OnRemoved)
	{
		return;
	}

	StopWaitingForGameplayTag(GameplayTag);

	FMGameplayTagCycleWaiter& Waiter = GameplayTagCycleWaiters.AddDefaulted_GetRef();
	Waiter.GameplayTag = GameplayTag;
	Waiter.OnRemoved = MoveTemp(OnRemoved);

	const int32 WaiterIndex = GameplayTagCycleWaiters.Num() - 1;
	if (AbilitySystemComponent->HasMatchingGameplayTag(GameplayTag))
	{
		BindGameplayTagRemovedWaiter(WaiterIndex);
		return;
	}

	Waiter.AddedHandle = AbilitySystemComponent
		->RegisterGameplayTagEvent(GameplayTag, EGameplayTagEventType::NewOrRemoved)
		.AddLambda([this, GameplayTag](const FGameplayTag, const int32 NewCount)
		{
			if (NewCount <= 0)
			{
				return;
			}

			const int32 FoundIndex = FindGameplayTagCycleWaiterIndex(GameplayTag);
			if (FoundIndex != INDEX_NONE)
			{
				BindGameplayTagRemovedWaiter(FoundIndex);
			}
		});
}

void UMGameplayAbility::StopWaitingForGameplayTag(const FGameplayTag& GameplayTag)
{
	const int32 WaiterIndex = FindGameplayTagCycleWaiterIndex(GameplayTag);
	if (WaiterIndex == INDEX_NONE)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetOwnerAbilitySystemComponent();
	const FMGameplayTagCycleWaiter Waiter = GameplayTagCycleWaiters[WaiterIndex];
	if (AbilitySystemComponent)
	{
		if (Waiter.AddedHandle.IsValid())
		{
			AbilitySystemComponent
				->RegisterGameplayTagEvent(Waiter.GameplayTag, EGameplayTagEventType::NewOrRemoved)
				.Remove(Waiter.AddedHandle);
		}
		if (Waiter.RemovedHandle.IsValid())
		{
			AbilitySystemComponent
				->RegisterGameplayTagEvent(Waiter.GameplayTag, EGameplayTagEventType::NewOrRemoved)
				.Remove(Waiter.RemovedHandle);
		}
	}

	GameplayTagCycleWaiters.RemoveAt(WaiterIndex);
}

int32 UMGameplayAbility::FindGameplayTagCycleWaiterIndex(const FGameplayTag& GameplayTag) const
{
	return GameplayTagCycleWaiters.IndexOfByPredicate([&GameplayTag](const FMGameplayTagCycleWaiter& Waiter)
	{
		return Waiter.GameplayTag == GameplayTag;
	});
}

void UMGameplayAbility::BindGameplayTagRemovedWaiter(const int32 WaiterIndex)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwnerAbilitySystemComponent();
	if (!AbilitySystemComponent || !GameplayTagCycleWaiters.IsValidIndex(WaiterIndex))
	{
		return;
	}

	FMGameplayTagCycleWaiter& Waiter = GameplayTagCycleWaiters[WaiterIndex];
	if (Waiter.AddedHandle.IsValid())
	{
		AbilitySystemComponent
			->RegisterGameplayTagEvent(Waiter.GameplayTag, EGameplayTagEventType::NewOrRemoved)
			.Remove(Waiter.AddedHandle);
		Waiter.AddedHandle.Reset();
	}

	if (Waiter.RemovedHandle.IsValid())
	{
		return;
	}

	Waiter.RemovedHandle = AbilitySystemComponent
		->RegisterGameplayTagEvent(Waiter.GameplayTag, EGameplayTagEventType::NewOrRemoved)
		.AddLambda([this, GameplayTag = Waiter.GameplayTag](const FGameplayTag, const int32 NewCount)
		{
			if (NewCount != 0)
			{
				return;
			}

			const int32 FoundIndex = FindGameplayTagCycleWaiterIndex(GameplayTag);
			if (FoundIndex == INDEX_NONE)
			{
				return;
			}

			TFunction<void()> Callback = MoveTemp(GameplayTagCycleWaiters[FoundIndex].OnRemoved);
			StopWaitingForGameplayTag(GameplayTag);
			if (Callback)
			{
				Callback();
			}
		});
}

void UMGameplayAbility::ClearGameplayTagCycleWaiters()
{
	while (GameplayTagCycleWaiters.Num() > 0)
	{
		StopWaitingForGameplayTag(GameplayTagCycleWaiters[0].GameplayTag);
	}
}

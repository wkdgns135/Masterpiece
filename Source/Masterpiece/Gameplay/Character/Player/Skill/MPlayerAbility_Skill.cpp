// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility_Skill.h"

#include "GameplayAbilitySpec.h"

const FGameplayTag& UMPlayerAbility_Skill::GetCurrentSkillTag() const
{
	return CurrentSkillTag;
}

const FGameplayTag& UMPlayerAbility_Skill::GetCurrentSlotTag() const
{
	return CurrentSlotTag;
}

int32 UMPlayerAbility_Skill::GetCurrentSkillRank() const
{
	return CurrentSkillRank;
}

const FGameplayTagContainer& UMPlayerAbility_Skill::GetCurrentSourceTags() const
{
	return CurrentSourceTags;
}

void UMPlayerAbility_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	RefreshRuntimeSkillContextFromSpec();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMPlayerAbility_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentSkillTag = FGameplayTag();
	CurrentSlotTag = FGameplayTag();
	CurrentSkillRank = 1;
	CurrentSourceTags.Reset();
}

void UMPlayerAbility_Skill::RefreshRuntimeSkillContextFromSpec()
{
	CurrentSkillTag = FGameplayTag();
	CurrentSlotTag = FGameplayTag();
	CurrentSkillRank = 1;
	CurrentSourceTags.Reset();

	const FGameplayAbilitySpec* CurrentSpec = GetCurrentAbilitySpec();
	if (!CurrentSpec)
	{
		return;
	}

	CurrentSkillRank = FMath::Max(1, CurrentSpec->Level);
	CurrentSourceTags.AppendTags(CurrentSpec->GetDynamicSpecSourceTags());

	const FGameplayTag SkillRootTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Skill")), false);
	const FGameplayTag AbilitySkillRootTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Skill")), false);
	for (const FGameplayTag& SourceTag : CurrentSourceTags)
	{
		if (!CurrentSkillTag.IsValid() && SkillRootTag.IsValid() && SourceTag.MatchesTag(SkillRootTag))
		{
			CurrentSkillTag = SourceTag;
		}

		if (!CurrentSlotTag.IsValid() && AbilitySkillRootTag.IsValid() && SourceTag.MatchesTag(AbilitySkillRootTag))
		{
			CurrentSlotTag = SourceTag;
		}
	}
}

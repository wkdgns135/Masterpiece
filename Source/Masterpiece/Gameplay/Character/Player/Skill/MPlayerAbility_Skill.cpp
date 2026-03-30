#include "MPlayerAbility_Skill.h"

#include "GameplayAbilitySpec.h"
#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"

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

UMSkillInstance* UMPlayerAbility_Skill::GetCurrentSkillInstance() const
{
	return CurrentSkillInstance;
}

UMSkillDefinition* UMPlayerAbility_Skill::GetCurrentSkillDefinition() const
{
	return CurrentSkillInstance ? CurrentSkillInstance->GetSkillDefinition() : nullptr;
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

	CurrentSkillInstance = nullptr;
	CurrentSkillTag = FGameplayTag();
	CurrentSlotTag = FGameplayTag();
	CurrentSkillRank = 1;
	CurrentSourceTags.Reset();
}

void UMPlayerAbility_Skill::RefreshRuntimeSkillContextFromSpec()
{
	CurrentSkillInstance = nullptr;
	CurrentSkillTag = FGameplayTag();
	CurrentSlotTag = FGameplayTag();
	CurrentSkillRank = 1;
	CurrentSourceTags.Reset();

	const FGameplayAbilitySpec* CurrentSpec = GetCurrentAbilitySpec();
	if (!CurrentSpec)
	{
		return;
	}

	CurrentSourceTags.AppendTags(CurrentSpec->GetDynamicSpecSourceTags());
	CurrentSkillInstance = Cast<UMSkillInstance>(CurrentSpec->SourceObject.Get());
	if (CurrentSkillInstance)
	{
		CurrentSkillTag = CurrentSkillInstance->GetSkillTag();
		CurrentSlotTag = CurrentSkillInstance->GetAssignedSlotTag();
		CurrentSkillRank = FMath::Max(1, CurrentSkillInstance->GetCurrentRank());
	}

	if (!CurrentSkillTag.IsValid())
	{
		const FGameplayTag SkillRootTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Skill")), false);
		for (const FGameplayTag& SourceTag : CurrentSourceTags)
		{
			if (SkillRootTag.IsValid() && SourceTag.MatchesTag(SkillRootTag))
			{
				CurrentSkillTag = SourceTag;
				break;
			}
		}
	}

	if (!CurrentSlotTag.IsValid())
	{
		const FGameplayTag AbilitySkillRootTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Skill")), false);
		for (const FGameplayTag& SourceTag : CurrentSourceTags)
		{
			if (AbilitySkillRootTag.IsValid() && SourceTag.MatchesTag(AbilitySkillRootTag))
			{
				CurrentSlotTag = SourceTag;
				break;
			}
		}
	}

	if (!CurrentSkillInstance)
	{
		CurrentSkillRank = FMath::Max(1, CurrentSpec->Level);
	}
}

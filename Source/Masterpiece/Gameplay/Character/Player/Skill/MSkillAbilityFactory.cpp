// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Character/Player/Skill/MSkillAbilityFactory.h"

#include "GameplayAbilitySpec.h"
#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Character/Player/Skill/MPlayerAbility_Skill.h"

bool UMSkillAbilityFactory::BuildAbilitySpec(const FMSkillDefinitionActive& SkillDefinition, const FGameplayTag& SlotTag, const int32 SkillRank, UObject* SourceObject, FGameplayAbilitySpec& OutSpec)
{
	const TSubclassOf<UMPlayerAbility_Skill> AbilityClass = SkillDefinition.AbilityClass.LoadSynchronous();
	if (!AbilityClass)
	{
		return false;
	}

	OutSpec = FGameplayAbilitySpec(AbilityClass, FMath::Max(1, SkillRank));
	OutSpec.SourceObject = SourceObject;

	if (SkillDefinition.SkillTag.IsValid())
	{
		OutSpec.GetDynamicSpecSourceTags().AddTag(SkillDefinition.SkillTag);
	}
	if (SkillDefinition.AbilityTag.IsValid())
	{
		OutSpec.GetDynamicSpecSourceTags().AddTag(SkillDefinition.AbilityTag);
	}
	if (SlotTag.IsValid())
	{
		OutSpec.GetDynamicSpecSourceTags().AddTag(SlotTag);
	}

	return true;
}

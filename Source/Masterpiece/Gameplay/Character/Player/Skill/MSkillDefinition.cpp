#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"

FGameplayTag UMSkillDefinition::GetSkillTag() const
{
	return GetDefinitionTag();
}

const TSoftObjectPtr<UTexture2D>& UMSkillDefinition::GetIcon() const
{
	return Icon;
}

const FText& UMSkillDefinition::GetDisplayName() const
{
	return DisplayName;
}

const FText& UMSkillDefinition::GetDescription() const
{
	return Description;
}

const TMap<FGameplayTag, int32>& UMSkillDefinition::GetPrerequisiteSkillRanks() const
{
	return PrerequisiteSkillRanks;
}

TArray<FGameplayTag> UMSkillDefinition::GetPrerequisiteSkillTags() const
{
	TArray<FGameplayTag> SkillTags;
	PrerequisiteSkillRanks.GetKeys(SkillTags);
	return SkillTags;
}

int32 UMSkillDefinition::GetRequiredCharacterLevel() const
{
	return RequiredCharacterLevel;
}

int32 UMSkillDefinition::GetMaxRank() const
{
	return MaxRank;
}

int32 UMSkillDefinition::GetCostPerRank() const
{
	return CostPerRank;
}

const TSoftObjectPtr<UAnimMontage>& UMSkillDefinitionActive::GetCastingMontage() const
{
	return CastingMontage;
}

const TSoftObjectPtr<UAnimMontage>& UMSkillDefinitionActive::GetSkillMontage() const
{
	return SkillMontage;
}

const TSoftClassPtr<UMPlayerAbility_Skill>& UMSkillDefinitionActive::GetAbilityClass() const
{
	return AbilityClass;
}


FGameplayTag UMSkillDefinitionActive::GetAbilityTag() const
{
	return AbilityTag;
}

FName UMSkillDefinitionActive::GetMontageSection() const
{
	return MontageSection;
}

float UMSkillDefinitionActive::GetDamageMultiplierBase() const
{
	return DamageMultiplierBase;
}

float UMSkillDefinitionActive::GetDamageMultiplierPerRank() const
{
	return DamageMultiplierPerRank;
}

float UMSkillDefinitionActive::GetCooldownSecondsBase() const
{
	return CooldownSecondsBase;
}

float UMSkillDefinitionActive::GetCooldownSecondsPerRank() const
{
	return CooldownSecondsPerRank;
}

float UMSkillDefinitionActive::GetManaCostBase() const
{
	return ManaCostBase;
}

float UMSkillDefinitionActive::GetManaCostPerRank() const
{
	return ManaCostPerRank;
}

float UMSkillDefinitionActive::GetCastTimeSeconds() const
{
	return CastTimeSeconds;
}

bool UMSkillDefinitionActive::IsPassiveDefinition() const
{
	return false;
}

FGameplayTag UMSkillDefinitionPassive::GetPassiveEffectTag() const
{
	return PassiveEffectTag;
}

float UMSkillDefinitionPassive::GetEffectValueBase() const
{
	return EffectValueBase;
}

float UMSkillDefinitionPassive::GetEffectValuePerRank() const
{
	return EffectValuePerRank;
}

bool UMSkillDefinitionPassive::IsPassiveDefinition() const
{
	return true;
}

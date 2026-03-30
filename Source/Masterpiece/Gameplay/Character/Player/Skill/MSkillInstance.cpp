#include "Gameplay/Character/Player/Skill/MSkillInstance.h"

bool UMSkillInstance::InitializeSkillInstance(UMSkillDefinition* InDefinition)
{
	if (!InDefinition)
	{
		return false;
	}

	InitializeDefinition(InDefinition);
	bUnlocked = true;
	CurrentRank = 1;
	AssignedSlotTag = FGameplayTag();
	return true;
}

UMSkillDefinition* UMSkillInstance::GetSkillDefinition() const
{
	return Cast<UMSkillDefinition>(Definition);
}

UMSkillDefinitionActive* UMSkillInstance::GetActiveDefinition() const
{
	return Cast<UMSkillDefinitionActive>(Definition);
}

UMSkillDefinitionPassive* UMSkillInstance::GetPassiveDefinition() const
{
	return Cast<UMSkillDefinitionPassive>(Definition);
}

FGameplayTag UMSkillInstance::GetSkillTag() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetSkillTag() : FGameplayTag();
}

const TSoftObjectPtr<UTexture2D>& UMSkillInstance::GetIcon() const
{
	static const TSoftObjectPtr<UTexture2D> EmptyIcon;

	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetIcon() : EmptyIcon;
}

const FText& UMSkillInstance::GetDisplayName() const
{
	static const FText EmptyText = FText::GetEmpty();

	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetDisplayName() : EmptyText;
}

const FText& UMSkillInstance::GetDescription() const
{
	static const FText EmptyText = FText::GetEmpty();

	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetDescription() : EmptyText;
}

TArray<FGameplayTag> UMSkillInstance::GetPrerequisiteSkillTags() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetPrerequisiteSkillTags() : TArray<FGameplayTag>();
}

const TMap<FGameplayTag, int32>& UMSkillInstance::GetPrerequisiteSkillRanks() const
{
	static const TMap<FGameplayTag, int32> EmptyPrerequisites;

	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetPrerequisiteSkillRanks() : EmptyPrerequisites;
}

int32 UMSkillInstance::GetRequiredCharacterLevel() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetRequiredCharacterLevel() : 1;
}

int32 UMSkillInstance::GetCostPerRank() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetCostPerRank() : 1;
}

int32 UMSkillInstance::GetMaxRank() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetMaxRank() : 1;
}

bool UMSkillInstance::IsPassive() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition && SkillDefinition->IsPassiveDefinition();
}

bool UMSkillInstance::IsUnlocked() const
{
	return bUnlocked;
}

bool UMSkillInstance::IsEquipped() const
{
	return AssignedSlotTag.IsValid();
}

int32 UMSkillInstance::GetCurrentRank() const
{
	return CurrentRank;
}

FGameplayTag UMSkillInstance::GetAssignedSlotTag() const
{
	return AssignedSlotTag;
}

void UMSkillInstance::SetUnlocked(const bool bInUnlocked)
{
	bUnlocked = bInUnlocked;
}

void UMSkillInstance::SetCurrentRank(const int32 InCurrentRank)
{
	CurrentRank = FMath::Clamp(InCurrentRank, 0, FMath::Max(1, GetMaxRank()));
}

void UMSkillInstance::SetAssignedSlotTag(const FGameplayTag InAssignedSlotTag)
{
	AssignedSlotTag = InAssignedSlotTag;
}

const UMSkillDefinition* UMSkillInstance::GetSkillDefinitionChecked() const
{
	return Cast<UMSkillDefinition>(Definition);
}

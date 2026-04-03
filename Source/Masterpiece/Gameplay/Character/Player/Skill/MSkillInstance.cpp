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
	RefreshSkillViewData();
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
	return SkillTag;
}

TSoftObjectPtr<UTexture2D> UMSkillInstance::GetIcon() const
{
	return Icon;
}

FText UMSkillInstance::GetDisplayName() const
{
	return DisplayName;
}

FText UMSkillInstance::GetDescription() const
{
	return Description;
}

TArray<FGameplayTag> UMSkillInstance::GetPrerequisiteSkillTags() const
{
	return ParentSkillTags;
}

TMap<FGameplayTag, int32> UMSkillInstance::GetPrerequisiteSkillRanks() const
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();
	return SkillDefinition ? SkillDefinition->GetPrerequisiteSkillRanks() : TMap<FGameplayTag, int32>();
}

int32 UMSkillInstance::GetRequiredCharacterLevel() const
{
	return RequiredCharacterLevel;
}

int32 UMSkillInstance::GetCostPerRank() const
{
	return CostPerRank;
}

int32 UMSkillInstance::GetMaxRank() const
{
	return MaxRank;
}

bool UMSkillInstance::IsPassive() const
{
	return bPassive;
}

bool UMSkillInstance::IsUnlocked() const
{
	return bUnlocked;
}

bool UMSkillInstance::IsEquipped() const
{
	return bEquipped;
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
	if (bUnlocked == bInUnlocked)
	{
		return;
	}

	bUnlocked = bInUnlocked;
	RefreshSkillViewData();
}

void UMSkillInstance::SetCurrentRank(const int32 InCurrentRank)
{
	const int32 NewRank = FMath::Clamp(InCurrentRank, 0, FMath::Max(1, GetMaxRank()));
	if (CurrentRank == NewRank)
	{
		return;
	}

	CurrentRank = NewRank;
	RefreshSkillViewData();
}

void UMSkillInstance::SetAssignedSlotTag(const FGameplayTag InAssignedSlotTag)
{
	if (AssignedSlotTag == InAssignedSlotTag)
	{
		return;
	}

	AssignedSlotTag = InAssignedSlotTag;
	RefreshSkillViewData();
}

const UMSkillDefinition* UMSkillInstance::GetSkillDefinitionChecked() const
{
	return Cast<UMSkillDefinition>(Definition);
}

void UMSkillInstance::RefreshSkillViewData()
{
	const UMSkillDefinition* SkillDefinition = GetSkillDefinitionChecked();

	SkillTag = SkillDefinition ? SkillDefinition->GetSkillTag() : FGameplayTag();
	Icon = SkillDefinition ? SkillDefinition->GetIcon() : TSoftObjectPtr<UTexture2D>();
	DisplayName = SkillDefinition ? SkillDefinition->GetDisplayName() : FText::GetEmpty();
	Description = SkillDefinition ? SkillDefinition->GetDescription() : FText::GetEmpty();
	RequiredCharacterLevel = SkillDefinition ? SkillDefinition->GetRequiredCharacterLevel() : 1;
	CostPerRank = SkillDefinition ? SkillDefinition->GetCostPerRank() : 1;
	MaxRank = SkillDefinition ? SkillDefinition->GetMaxRank() : 1;
	ParentSkillTags = SkillDefinition ? SkillDefinition->GetPrerequisiteSkillTags() : TArray<FGameplayTag>();
	bPassive = SkillDefinition && SkillDefinition->IsPassiveDefinition();
	bEquipped = AssignedSlotTag.IsValid();
	
	BroadcastDefinitionInstanceChanged();
}

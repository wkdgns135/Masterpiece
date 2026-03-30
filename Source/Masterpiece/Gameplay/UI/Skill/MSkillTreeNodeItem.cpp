#include "Gameplay/UI/Skill/MSkillTreeNodeItem.h"

#include "Gameplay/Character/Player/Skill/MSkillInstance.h"

void UMSkillTreeNodeItem::SetSkillInstance(UMSkillInstance* InSkillInstance)
{
	SkillInstance = InSkillInstance;
	RefreshFromSkillInstance();
}

UMSkillInstance* UMSkillTreeNodeItem::GetSkillInstance() const
{
	return SkillInstance;
}

void UMSkillTreeNodeItem::RefreshFromSkillInstance()
{
	SkillTag = SkillInstance ? SkillInstance->GetSkillTag() : FGameplayTag();
	Icon = SkillInstance ? SkillInstance->GetIcon() : TSoftObjectPtr<UTexture2D>();
	DisplayName = SkillInstance ? SkillInstance->GetDisplayName() : FText::GetEmpty();
	Description = SkillInstance ? SkillInstance->GetDescription() : FText::GetEmpty();
	RequiredCharacterLevel = SkillInstance ? SkillInstance->GetRequiredCharacterLevel() : 1;
	CostPerRank = SkillInstance ? SkillInstance->GetCostPerRank() : 1;
	MaxRank = SkillInstance ? SkillInstance->GetMaxRank() : 1;
	ParentSkillTags = SkillInstance ? SkillInstance->GetPrerequisiteSkillTags() : TArray<FGameplayTag>();
	bPassive = SkillInstance && SkillInstance->IsPassive();
	bUnlocked = !SkillInstance || SkillInstance->IsUnlocked();
	bEquipped = SkillInstance && SkillInstance->IsEquipped();
	CurrentRank = SkillInstance ? SkillInstance->GetCurrentRank() : 1;
	AssignedSlotTag = SkillInstance ? SkillInstance->GetAssignedSlotTag() : FGameplayTag();
}

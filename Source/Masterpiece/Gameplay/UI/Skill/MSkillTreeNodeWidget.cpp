#include "Gameplay/UI/Skill/MSkillTreeNodeWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "Gameplay/Definition/MDefinitionInstance.h"

void UMSkillTreeNodeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	RefreshSkillData();
}

void UMSkillTreeNodeWidget::SetSkillInstance(UMSkillInstance* InSkillInstance)
{
	SkillInstance = InSkillInstance;
	RefreshSkillData();
	K2_OnSkillInstanceSet(SkillInstance);
}

UMSkillInstance* UMSkillTreeNodeWidget::GetSkillInstance() const
{
	return SkillInstance;
}

UImage* UMSkillTreeNodeWidget::GetDragHandleImage_Implementation() const
{
	return SkillIcon.Get();
}

UMDefinitionInstance* UMSkillTreeNodeWidget::GetDraggableDefinitionInstance_Implementation() const
{
	return GetSkillInstance();
}

bool UMSkillTreeNodeWidget::CanDragDefinitionInstance_Implementation() const
{
	return GetSkillInstance() != nullptr;
}

bool UMSkillTreeNodeWidget::CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const
{
	return Super::CanDropDefinitionInstance_Implementation(DefinitionInstance);
}

void UMSkillTreeNodeWidget::RefreshSkillData()
{
	if (!SkillInstance)
	{
		return;
	}
	
	SkillIcon->SetBrushFromTexture(SkillInstance->GetIcon().LoadSynchronous());
	SkillName->SetText(SkillInstance->GetDisplayName());
	SkillRank->SetText(FText::AsNumber(SkillInstance->GetCurrentRank()));
}

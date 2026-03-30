#include "Gameplay/UI/Skill/MSkillTreeNodeWidget.h"

#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "Gameplay/UI/Skill/MSkillTreeNodeItem.h"

void UMSkillTreeNodeWidget::SetNodeItem(UMSkillTreeNodeItem* InNodeItem)
{
	NodeItem = InNodeItem;
	K2_OnNodeItemSet(InNodeItem);
	K2_OnSkillInstanceSet(GetSkillInstance());
}

UMSkillTreeNodeItem* UMSkillTreeNodeWidget::GetNodeItem() const
{
	return NodeItem;
}

UMSkillInstance* UMSkillTreeNodeWidget::GetSkillInstance() const
{
	return NodeItem ? NodeItem->GetSkillInstance() : nullptr;
}

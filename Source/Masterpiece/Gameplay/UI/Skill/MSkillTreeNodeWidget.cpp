// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/Skill/MSkillTreeNodeWidget.h"

#include "Gameplay/UI/Skill/MSkillTreeNodeItem.h"

void UMSkillTreeNodeWidget::SetNodeItem(UMSkillTreeNodeItem* InNodeItem)
{
	NodeItem = InNodeItem;
	K2_OnNodeItemSet(InNodeItem);
}

UMSkillTreeNodeItem* UMSkillTreeNodeWidget::GetNodeItem() const
{
	return NodeItem;
}

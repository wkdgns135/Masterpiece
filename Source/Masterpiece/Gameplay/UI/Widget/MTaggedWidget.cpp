// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/Widget/MTaggedWidget.h"

FGameplayTag UMTaggedWidget::GetWidgetTag() const
{
	return WidgetTag;
}

bool UMTaggedWidget::MatchesWidgetTag(const FGameplayTag InWidgetTag) const
{
	return WidgetTag.IsValid() && WidgetTag == InWidgetTag;
}

void UMTaggedWidget::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (WidgetTag.IsValid())
	{
		TagContainer.AddTag(WidgetTag);
	}
}

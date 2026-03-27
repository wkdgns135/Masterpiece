// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/MUIGameplayTags.h"

namespace MUIGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(UI_Action_Escape, "UI.Action.Escape");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_GameHUD, "UI.Widget.GameHUD");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_GameMenu, "UI.Widget.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_Modal, "UI.Widget.Modal");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_Notify, "UI.Widget.Notify");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_GameMenu_SkillTree, "UI.Widget.GameMenu.SkillTree");

	bool TryResolveWidgetLayerTag(const FGameplayTag& WidgetTag, FGameplayTag& OutLayerTag)
	{
		if (!WidgetTag.IsValid())
		{
			OutLayerTag = FGameplayTag();
			return false;
		}

		if (WidgetTag.MatchesTag(UI_Widget_Modal))
		{
			OutLayerTag = UI_Widget_Modal;
			return true;
		}

		if (WidgetTag.MatchesTag(UI_Widget_Notify))
		{
			OutLayerTag = UI_Widget_Notify;
			return true;
		}

		if (WidgetTag.MatchesTag(UI_Widget_GameMenu))
		{
			OutLayerTag = UI_Widget_GameMenu;
			return true;
		}

		if (WidgetTag.MatchesTag(UI_Widget_GameHUD))
		{
			OutLayerTag = UI_Widget_GameHUD;
			return true;
		}

		OutLayerTag = FGameplayTag();
		return false;
	}

	int32 GetLayerBaseZOrder(const FGameplayTag& LayerTag)
	{
		if (LayerTag == UI_Widget_GameHUD)
		{
			return 0;
		}

		if (LayerTag == UI_Widget_GameMenu)
		{
			return 100;
		}

		if (LayerTag == UI_Widget_Notify)
		{
			return 500;
		}

		if (LayerTag == UI_Widget_Modal)
		{
			return 1000;
		}

		return 0;
	}

	int32 GetWidgetBaseZOrder(const FGameplayTag& WidgetTag)
	{
		FGameplayTag LayerTag;
		return TryResolveWidgetLayerTag(WidgetTag, LayerTag) ? GetLayerBaseZOrder(LayerTag) : 0;
	}
}

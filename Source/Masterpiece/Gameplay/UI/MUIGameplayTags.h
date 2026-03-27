// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace MUIGameplayTags
{
	MASTERPIECE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Action_Escape);
	MASTERPIECE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_GameHUD);
	MASTERPIECE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_GameMenu);
	MASTERPIECE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_Modal);
	MASTERPIECE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_Notify);
	MASTERPIECE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_GameMenu_SkillTree);

	MASTERPIECE_API bool TryResolveWidgetLayerTag(const FGameplayTag& WidgetTag, FGameplayTag& OutLayerTag);
	MASTERPIECE_API int32 GetLayerBaseZOrder(const FGameplayTag& LayerTag);
	MASTERPIECE_API int32 GetWidgetBaseZOrder(const FGameplayTag& WidgetTag);
}

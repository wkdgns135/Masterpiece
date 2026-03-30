// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/MUIDeveloperSettings.h"

#include "Gameplay/UI/MUIWidgetRegistry.h"

FName UMUIDeveloperSettings::GetCategoryName() const
{
	return TEXT("Game");
}

bool UMUIDeveloperSettings::FindWidgetRegistryByLayerTag(const FGameplayTag LayerTag, TSoftObjectPtr<UMUIWidgetRegistry>& OutRegistry) const
{
	const TSoftObjectPtr<UMUIWidgetRegistry>* Registry = LayerRegistries.Find(LayerTag);
	if (!Registry)
	{
		OutRegistry.Reset();
		return false;
	}

	OutRegistry = *Registry;
	return true;
}

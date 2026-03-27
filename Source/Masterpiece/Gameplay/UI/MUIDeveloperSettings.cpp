// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/MUIDeveloperSettings.h"

#include "Gameplay/UI/MUIWidgetRegistryDataAsset.h"

FName UMUIDeveloperSettings::GetCategoryName() const
{
	return TEXT("Game");
}

bool UMUIDeveloperSettings::FindWidgetRegistryAssetByLayerTag(const FGameplayTag LayerTag, TSoftObjectPtr<UMUIWidgetRegistryDataAsset>& OutRegistryAsset) const
{
	const TSoftObjectPtr<UMUIWidgetRegistryDataAsset>* RegistryAsset = LayerRegistries.Find(LayerTag);
	if (!RegistryAsset)
	{
		OutRegistryAsset.Reset();
		return false;
	}

	OutRegistryAsset = *RegistryAsset;
	return true;
}

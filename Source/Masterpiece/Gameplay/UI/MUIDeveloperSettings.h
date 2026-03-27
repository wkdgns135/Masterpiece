// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "MUIDeveloperSettings.generated.h"

class UMUIWidgetRegistryDataAsset;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="UI"))
class MASTERPIECE_API UMUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override;

	bool FindWidgetRegistryAssetByLayerTag(FGameplayTag LayerTag, TSoftObjectPtr<UMUIWidgetRegistryDataAsset>& OutRegistryAsset) const;

private:
	UPROPERTY(Config, EditAnywhere, Category="UI", meta=(Categories="UI.Widget"))
	TMap<FGameplayTag, TSoftObjectPtr<UMUIWidgetRegistryDataAsset>> LayerRegistries;
};

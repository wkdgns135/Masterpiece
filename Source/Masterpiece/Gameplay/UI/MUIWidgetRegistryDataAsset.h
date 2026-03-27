// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "MUIWidgetRegistryDataAsset.generated.h"

class UMTaggedWidget;

UCLASS(BlueprintType)
class MASTERPIECE_API UMUIWidgetRegistryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI")
	TSubclassOf<UMTaggedWidget> FindWidgetClassByTag(FGameplayTag WidgetTag) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	TMap<FGameplayTag, TSoftClassPtr<UMTaggedWidget>> WidgetEntries;
};

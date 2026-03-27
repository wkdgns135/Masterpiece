// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "MTaggedWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMTaggedWidget : public UUserWidget, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI")
	FGameplayTag GetWidgetTag() const;

	UFUNCTION(BlueprintPure, Category="UI")
	bool MatchesWidgetTag(FGameplayTag InWidgetTag) const;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI", meta=(Categories="UI.Widget"))
	FGameplayTag WidgetTag;
};

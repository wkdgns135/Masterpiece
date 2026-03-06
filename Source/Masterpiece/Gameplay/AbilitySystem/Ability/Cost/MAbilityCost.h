// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MAbilityCost.generated.h"

class UMGameplayAbility;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Ability|Cost")
	bool CheckCost(const UMGameplayAbility* Ability) const;
	virtual bool CheckCost_Implementation(const UMGameplayAbility* Ability) const;

	UFUNCTION(BlueprintNativeEvent, Category="Ability|Cost")
	void ApplyCost(const UMGameplayAbility* Ability) const;
	virtual void ApplyCost_Implementation(const UMGameplayAbility* Ability) const;
};

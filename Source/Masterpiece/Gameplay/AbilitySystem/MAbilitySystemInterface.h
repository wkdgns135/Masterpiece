// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MAbilitySystemInterface.generated.h"

struct FGameplayTag;
class UMAbilitySystemComponent;

UINTERFACE(MinimalAPI, NotBlueprintable)
class UMAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTERPIECE_API IMAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UMAbilitySystemComponent* GetAbilitySystemComponent() const = 0;

	bool TryActivateAbilityByTag(const FGameplayTag& AbilityTag) const;
};

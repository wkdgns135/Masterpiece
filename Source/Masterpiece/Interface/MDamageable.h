// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTERPIECE_API IMDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Damageable")
	virtual void ApplyDamage(float Damage) = 0;
};

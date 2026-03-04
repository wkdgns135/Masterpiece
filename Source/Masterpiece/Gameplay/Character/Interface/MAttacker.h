// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MAttacker.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UMAttacker : public UInterface
{
	GENERATED_BODY()
};

class MASTERPIECE_API IMAttacker
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Attacker")
	virtual void DoAttackTrace(FName DamageSourceBone) = 0;
};

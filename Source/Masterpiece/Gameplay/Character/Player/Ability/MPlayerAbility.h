// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AbilitySystem/Ability/MGameplayAbility.h"
#include "MPlayerAbility.generated.h"

class AMPlayerCharacterBase;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMPlayerAbility : public UMGameplayAbility
{
	GENERATED_BODY()
	
public:
	AMPlayerCharacterBase* GetPlayerCharacter() const;
};

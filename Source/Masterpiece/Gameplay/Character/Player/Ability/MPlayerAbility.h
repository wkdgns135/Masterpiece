// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AbilitySystem/Ability/MGameplayAbility.h"
#include "MPlayerAbility.generated.h"

class UMPlayerCombatComponent;
class AMGameplayPlayerController;
class UMPlayerMovementComponent;
class AMPlayerCharacterBase;
struct FGameplayEventData;
struct FGameplayTag;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMPlayerAbility : public UMGameplayAbility
{
	GENERATED_BODY()
	
public:
	AMPlayerCharacterBase* GetMPlayerCharacter() const;
	AMGameplayPlayerController* GetMGameplayPlayerController() const;
	UMPlayerMovementComponent* GetMPlayerMovementComponent() const;
	UMPlayerCombatComponent* GetMPlayerCombatComponent() const;
	bool SendGameplayEventToOwner(const FGameplayTag& EventTag, const FGameplayEventData& Payload) const;
	
};

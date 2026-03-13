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
	UFUNCTION( BlueprintCallable, Category="Ability")
	AMPlayerCharacterBase* GetMPlayerCharacter() const;
	
	UFUNCTION( BlueprintCallable, Category="Ability")
	AMGameplayPlayerController* GetMGameplayPlayerController() const;
	
	UFUNCTION( BlueprintCallable, Category="Ability")
	UMPlayerMovementComponent* GetMPlayerMovementComponent() const;
	
	UFUNCTION( BlueprintCallable, Category="Ability")
	UMPlayerCombatComponent* GetMPlayerCombatComponent() const;
	
	UFUNCTION( BlueprintCallable, Category="Ability")
	bool SendGameplayEventToOwner(const FGameplayTag& EventTag, const FGameplayEventData& Payload) const;
	
};

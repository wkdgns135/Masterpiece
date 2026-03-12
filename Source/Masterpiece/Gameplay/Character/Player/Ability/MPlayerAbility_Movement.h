// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerAbility.h"
#include "MPlayerAbility_Movement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MASTERPIECE_API UMPlayerAbility_Movement : public UMPlayerAbility
{
	GENERATED_BODY()
	
public:
	UMPlayerAbility_Movement();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};

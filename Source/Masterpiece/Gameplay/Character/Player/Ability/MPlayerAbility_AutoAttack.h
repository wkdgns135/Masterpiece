// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerAbility.h"
#include "MPlayerAbility_AutoAttack.generated.h"

/**
 * 
 */
UCLASS()
class MASTERPIECE_API UMPlayerAbility_AutoAttack : public UMPlayerAbility
{
	GENERATED_BODY()

public:
	UMPlayerAbility_AutoAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void ExecuteAutoAttackStep();
	void HandleMovementFinished(bool bReachedTarget);
	void HandleAttackFinished();
	
	UPROPERTY(Transient)
	TObjectPtr<const AActor> CurrentTargetActor;
};

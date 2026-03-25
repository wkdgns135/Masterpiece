// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Delegates/Delegate.h"
#include "GameplayTagContainer.h"
#include "MGameplayAbility.generated.h"


UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UMAbilitySystemComponent;
	
public:
	UMGameplayAbility();

	UFUNCTION(BlueprintPure, Category="Ability")
	const FGameplayTag& GetAbilityTag() const;

protected:
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure, Category="Ability|Common")
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category="Ability|Common")
	AActor* GetOwnerActor() const;

	UFUNCTION(BlueprintPure, Category="Ability|Common")
	AActor* GetAvatarActor() const;

	UFUNCTION(BlueprintPure, Category="Ability|Common")
	bool HasEnoughCost() const;

	UFUNCTION(BlueprintPure, Category="Ability|Common")
	bool IsAbilityOnCooldown() const;

	UFUNCTION(BlueprintCallable, Category="Ability|Common")
	bool CommitAbilitySafely();

	UFUNCTION(BlueprintCallable, Category="Ability|Common")
	void EndAbilityAsSuccess(bool bReplicateEndAbility = true);

	UFUNCTION(BlueprintCallable, Category="Ability|Common")
	void EndAbilityAsCancelled(bool bReplicateEndAbility = true);
	
	UFUNCTION( BlueprintCallable, Category="Ability|Common")
	bool SendGameplayEventToOwner(const FGameplayTag& EventTag, const FGameplayEventData& Payload) const;

protected:
	// Ability identity tag used for ASC activation lookup.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Identity")
	FGameplayTag AbilityTag;

	// Additional costs layered on top of native GAS cost handling.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Ability|Cost")
	TArray<TObjectPtr<class UMAbilityCost>> AdditionalCosts;

};

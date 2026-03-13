// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Delegates/Delegate.h"
#include "GameplayTagContainer.h"
#include "MGameplayAbility.generated.h"

struct FMGameplayTagCycleWaiter
{
	FGameplayTag GameplayTag;
	FDelegateHandle AddedHandle;
	FDelegateHandle RemovedHandle;
	TFunction<void()> OnRemoved;
};

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

	void WaitForGameplayTagToBeAddedThenRemoved(const FGameplayTag& GameplayTag, TFunction<void()> OnRemoved);

	template <typename UserClass>
	void WaitForGameplayTagToBeAddedThenRemoved(const FGameplayTag& GameplayTag, UserClass* Object, void (UserClass::*Method)())
	{
		if (!Object || !Method)
		{
			return;
		}

		WaitForGameplayTagToBeAddedThenRemoved(GameplayTag, [Object, Method]()
		{
			(Object->*Method)();
		});
	}

	void StopWaitingForGameplayTag(const FGameplayTag& GameplayTag);

protected:
	// Ability identity tag used for ASC activation lookup.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Identity")
	FGameplayTag AbilityTag;

	// Additional costs layered on top of native GAS cost handling.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Ability|Cost")
	TArray<TObjectPtr<class UMAbilityCost>> AdditionalCosts;

private:
	int32 FindGameplayTagCycleWaiterIndex(const FGameplayTag& GameplayTag) const;
	void BindGameplayTagRemovedWaiter(int32 WaiterIndex);
	void ClearGameplayTagCycleWaiters();

	TArray<FMGameplayTagCycleWaiter> GameplayTagCycleWaiters;
};

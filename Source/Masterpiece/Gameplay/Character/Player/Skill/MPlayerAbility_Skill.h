#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Character/Player/Ability/MPlayerAbility.h"
#include "MPlayerAbility_Skill.generated.h"

class UMSkillDefinition;
class UMSkillInstance;

UCLASS(Abstract)
class MASTERPIECE_API UMPlayerAbility_Skill : public UMPlayerAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Skill")
	const FGameplayTag& GetCurrentSkillTag() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	const FGameplayTag& GetCurrentSlotTag() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	int32 GetCurrentSkillRank() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	const FGameplayTagContainer& GetCurrentSourceTags() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	UMSkillInstance* GetCurrentSkillInstance() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	UMSkillDefinition* GetCurrentSkillDefinition() const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void RefreshRuntimeSkillContextFromSpec();

private:
	UPROPERTY(Transient)
	TObjectPtr<UMSkillInstance> CurrentSkillInstance;

	UPROPERTY(Transient)
	FGameplayTag CurrentSkillTag;

	UPROPERTY(Transient)
	FGameplayTag CurrentSlotTag;

	UPROPERTY(Transient)
	int32 CurrentSkillRank = 1;

	UPROPERTY(Transient)
	FGameplayTagContainer CurrentSourceTags;
};

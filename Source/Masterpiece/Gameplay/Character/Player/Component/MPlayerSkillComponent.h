// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "MPlayerSkillComponent.generated.h"

struct FMSkillDefinitionActive;

class UAbilitySystemComponent;
class UMSkillTreeDataAsset;

DECLARE_MULTICAST_DELEGATE(FOnPlayerSkillLoadoutChanged);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMPlayerSkillComponent();

	FOnPlayerSkillLoadoutChanged OnSkillLoadoutChanged;

	const UMSkillTreeDataAsset* GetSkillTreeAsset() const;

	UFUNCTION(BlueprintCallable, Category="Skill")
	bool EquipSkillToSlot(const FGameplayTag& SkillTag, const FGameplayTag& SlotTag, int32 SkillRank = 1);

	UFUNCTION(BlueprintCallable, Category="Skill")
	bool UnequipSkillSlot(const FGameplayTag& SlotTag);

	UFUNCTION(BlueprintPure, Category="Skill")
	bool IsSkillSlotEquipped(const FGameplayTag& SlotTag) const;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UAbilitySystemComponent* ResolveAbilitySystemComponent() const;
	const UMSkillTreeDataAsset* ResolveSkillTreeAsset() const;
	const FMSkillDefinitionActive* FindActiveSkillDefinitionByTag(const FGameplayTag& SkillTag) const;
	bool ClearEquippedSlotInternal(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& SlotTag);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UMSkillTreeDataAsset> SkillTreeAsset;

	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> EquippedSkillHandlesBySlot;
};

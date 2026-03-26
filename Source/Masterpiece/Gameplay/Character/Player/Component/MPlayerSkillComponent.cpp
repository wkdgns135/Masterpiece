// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Gameplay/Character/MAbilityCharacterBase.h"
#include "Gameplay/Character/Player/Skill/MSkillAbilityFactory.h"
#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Character/Player/Skill/MSkillTreeDataAsset.h"
#include "InstancedStruct.h"
#include "StructUtils/InstancedStruct.h"

UMPlayerSkillComponent::UMPlayerSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMPlayerSkillComponent::EquipSkillToSlot(const FGameplayTag& SkillTag, const FGameplayTag& SlotTag, const int32 SkillRank)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !SkillTag.IsValid())
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	const FMSkillDefinitionActive* SkillDefinition = FindActiveSkillDefinitionByTag(SkillTag);
	if (!AbilitySystemComponent || !SkillDefinition)
	{
		return false;
	}

	const FGameplayTag ActualSlotTag = SlotTag.IsValid() ? SlotTag : SkillDefinition->AbilityTag;
	if (!ActualSlotTag.IsValid())
	{
		return false;
	}

	ClearEquippedSlotInternal(AbilitySystemComponent, ActualSlotTag);

	FGameplayAbilitySpec AbilitySpec;
	if (!UMSkillAbilityFactory::BuildAbilitySpec(*SkillDefinition, ActualSlotTag, SkillRank, this, AbilitySpec))
	{
		return false;
	}

	const FGameplayAbilitySpecHandle NewHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
	if (!NewHandle.IsValid())
	{
		return false;
	}

	EquippedSkillHandlesBySlot.FindOrAdd(ActualSlotTag) = NewHandle;
	return true;
}

bool UMPlayerSkillComponent::UnequipSkillSlot(const FGameplayTag& SlotTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !SlotTag.IsValid())
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	return ClearEquippedSlotInternal(AbilitySystemComponent, SlotTag);
}

bool UMPlayerSkillComponent::IsSkillSlotEquipped(const FGameplayTag& SlotTag) const
{
	if (!SlotTag.IsValid())
	{
		return false;
	}

	if (const FGameplayAbilitySpecHandle* Handle = EquippedSkillHandlesBySlot.Find(SlotTag))
	{
		return Handle->IsValid();
	}

	return false;
}

void UMPlayerSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EquippedSkillHandlesBySlot.Empty();

	Super::EndPlay(EndPlayReason);
}

UAbilitySystemComponent* UMPlayerSkillComponent::ResolveAbilitySystemComponent() const
{
	const AMAbilityCharacterBase* AbilityCharacter = Cast<AMAbilityCharacterBase>(GetOwner());
	return AbilityCharacter ? AbilityCharacter->GetAbilitySystemComponent() : nullptr;
}

UMSkillTreeDataAsset* UMPlayerSkillComponent::ResolveSkillTreeAsset() const
{
	return SkillTreeAsset.LoadSynchronous();
}

const FMSkillDefinitionActive* UMPlayerSkillComponent::FindActiveSkillDefinitionByTag(const FGameplayTag& SkillTag) const
{
	const UMSkillTreeDataAsset* SkillTree = ResolveSkillTreeAsset();
	if (!SkillTree)
	{
		return nullptr;
	}

	for (const FInstancedStruct& SkillData : SkillTree->Skills)
	{
		const FMSkillDefinitionActive* ActiveSkillDefinition = SkillData.GetPtr<FMSkillDefinitionActive>();
		if (!ActiveSkillDefinition)
		{
			continue;
		}

		if (ActiveSkillDefinition->SkillTag.MatchesTagExact(SkillTag))
		{
			return ActiveSkillDefinition;
		}
	}

	return nullptr;
}

bool UMPlayerSkillComponent::ClearEquippedSlotInternal(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& SlotTag)
{
	if (!AbilitySystemComponent || !SlotTag.IsValid())
	{
		return false;
	}

	if (FGameplayAbilitySpecHandle* ExistingHandle = EquippedSkillHandlesBySlot.Find(SlotTag))
	{
		if (ExistingHandle->IsValid())
		{
			AbilitySystemComponent->ClearAbility(*ExistingHandle);
		}
		EquippedSkillHandlesBySlot.Remove(SlotTag);
		return true;
	}

	return false;
}

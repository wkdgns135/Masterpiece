#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"

#include "AbilitySystemComponent.h"
#include "Engine/GameInstance.h"
#include "GameplayAbilitySpec.h"
#include "Gameplay/Character/MAbilityCharacterBase.h"
#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Character/Player/Skill/MPlayerAbility_Skill.h"
#include "Gameplay/Character/Player/Skill/MSkillTreeDataAsset.h"
#include "Gameplay/Definition/MDefinitionCollectionDataAsset.h"
#include "Gameplay/Definition/MDefinitionSubsystem.h"

namespace
{
bool BuildAbilitySpecFromSkillInstance(const UMSkillInstance& SkillInstance, const FGameplayTag& SlotTag, FGameplayAbilitySpec& OutSpec)
{
	const UMSkillDefinitionActive* SkillDefinition = SkillInstance.GetActiveDefinition();
	if (!SkillDefinition)
	{
		return false;
	}

	const TSubclassOf<UMPlayerAbility_Skill> AbilityClass = SkillDefinition->GetAbilityClass().LoadSynchronous();
	if (!AbilityClass)
	{
		return false;
	}

	OutSpec = FGameplayAbilitySpec(AbilityClass, FMath::Max(1, SkillInstance.GetCurrentRank()));
	OutSpec.SourceObject = const_cast<UMSkillInstance*>(&SkillInstance);

	if (SkillInstance.GetSkillTag().IsValid())
	{
		OutSpec.GetDynamicSpecSourceTags().AddTag(SkillInstance.GetSkillTag());
	}

	if (SkillDefinition->GetAbilityTag().IsValid())
	{
		OutSpec.GetDynamicSpecSourceTags().AddTag(SkillDefinition->GetAbilityTag());
	}

	if (SlotTag.IsValid())
	{
		OutSpec.GetDynamicSpecSourceTags().AddTag(SlotTag);
	}

	return true;
}
}

UMPlayerSkillComponent::UMPlayerSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkillState();
}

bool UMPlayerSkillComponent::GetSkillInstances(TArray<UMSkillInstance*>& OutSkillInstances) const
{
	if (!EnsureSkillStateInitialized())
	{
		OutSkillInstances.Reset();
		return false;
	}

	OutSkillInstances.Reset(SkillInstances.Num());
	for (const TPair<FGameplayTag, TObjectPtr<UMSkillInstance>>& Pair : SkillInstances)
	{
		if (Pair.Value)
		{
			OutSkillInstances.Add(Pair.Value.Get());
		}
	}

	return OutSkillInstances.Num() > 0;
}

UMSkillInstance* UMPlayerSkillComponent::GetSkillInstance(const FGameplayTag& SkillTag) const
{
	if (!SkillTag.IsValid() || !EnsureSkillStateInitialized())
	{
		return nullptr;
	}

	if (const TObjectPtr<UMSkillInstance>* SkillInstance = SkillInstances.Find(SkillTag))
	{
		return SkillInstance->Get();
	}

	return nullptr;
}

FGameplayTag UMPlayerSkillComponent::GetAssignedSkillTag(const FGameplayTag& SlotTag) const
{
	if (!SlotTag.IsValid() || !EnsureSkillStateInitialized())
	{
		return FGameplayTag();
	}

	if (const FGameplayTag* EquippedSkillTag = EquippedSkillTagsBySlot.Find(SlotTag))
	{
		return *EquippedSkillTag;
	}

	return FGameplayTag();
}

bool UMPlayerSkillComponent::EquipSkillToSlot(const FGameplayTag& SkillTag, const FGameplayTag& SlotTag, const int32 SkillRank)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !SkillTag.IsValid() || !EnsureSkillStateInitialized())
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	UMSkillInstance* SkillInstance = GetSkillInstance(SkillTag);
	const UMSkillDefinitionActive* SkillDefinition = SkillInstance ? SkillInstance->GetActiveDefinition() : nullptr;
	if (!AbilitySystemComponent || !SkillDefinition || !SkillInstance || !SkillInstance->IsUnlocked())
	{
		return false;
	}

	const FGameplayTag ActualSlotTag = SlotTag.IsValid() ? SlotTag : SkillDefinition->GetAbilityTag();
	if (!ActualSlotTag.IsValid())
	{
		return false;
	}

	SkillInstance->SetCurrentRank(FMath::Max(SkillInstance->GetCurrentRank(), SkillRank));

	TArray<FGameplayTag> ChangedSlotTags;
	const FGameplayTag PreviousAssignedSlotTag = SkillInstance->GetAssignedSlotTag();
	if (PreviousAssignedSlotTag.IsValid() && PreviousAssignedSlotTag != ActualSlotTag)
	{
		FGameplayTag RemovedSkillTag;
		if (ClearEquippedSlotInternal(AbilitySystemComponent, PreviousAssignedSlotTag, &RemovedSkillTag))
		{
			if (UMSkillInstance* PreviousInstance = GetSkillInstance(RemovedSkillTag))
			{
				PreviousInstance->SetAssignedSlotTag(FGameplayTag());
			}

			ChangedSlotTags.AddUnique(PreviousAssignedSlotTag);
		}
	}

	FGameplayTag ReplacedSkillTag;
	if (ClearEquippedSlotInternal(AbilitySystemComponent, ActualSlotTag, &ReplacedSkillTag))
	{
		if (UMSkillInstance* ReplacedInstance = GetSkillInstance(ReplacedSkillTag))
		{
			ReplacedInstance->SetAssignedSlotTag(FGameplayTag());
		}

		ChangedSlotTags.AddUnique(ActualSlotTag);
	}

	FGameplayAbilitySpec AbilitySpec;
	if (!BuildAbilitySpecFromSkillInstance(*SkillInstance, ActualSlotTag, AbilitySpec))
	{
		return false;
	}

	const FGameplayAbilitySpecHandle NewHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
	if (!NewHandle.IsValid())
	{
		return false;
	}

	EquippedSkillHandlesBySlot.FindOrAdd(ActualSlotTag) = NewHandle;
	EquippedSkillTagsBySlot.FindOrAdd(ActualSlotTag) = SkillTag;
	SkillInstance->SetAssignedSlotTag(ActualSlotTag);

	ChangedSlotTags.AddUnique(ActualSlotTag);
	BroadcastSkillStateChanged(ChangedSlotTags);
	return true;
}

bool UMPlayerSkillComponent::UnequipSkillSlot(const FGameplayTag& SlotTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !SlotTag.IsValid() || !EnsureSkillStateInitialized())
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	FGameplayTag RemovedSkillTag;
	const bool bRemoved = ClearEquippedSlotInternal(AbilitySystemComponent, SlotTag, &RemovedSkillTag);
	if (!bRemoved)
	{
		return false;
	}

	if (UMSkillInstance* SkillInstance = GetSkillInstance(RemovedSkillTag))
	{
		SkillInstance->SetAssignedSlotTag(FGameplayTag());
	}

	TArray<FGameplayTag> ChangedSlotTags;
	ChangedSlotTags.Add(SlotTag);
	BroadcastSkillStateChanged(ChangedSlotTags);
	return true;
}

bool UMPlayerSkillComponent::IsSkillSlotEquipped(const FGameplayTag& SlotTag) const
{
	if (!SlotTag.IsValid() || !EnsureSkillStateInitialized())
	{
		return false;
	}

	const FGameplayTag* SkillTag = EquippedSkillTagsBySlot.Find(SlotTag);
	return SkillTag && SkillTag->IsValid();
}

void UMPlayerSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SkillInstances.Empty();
	EquippedSkillTagsBySlot.Empty();
	EquippedSkillHandlesBySlot.Empty();
	LoadedSkillTreeDefinition = nullptr;
	bSkillStateLoading = false;
	bSkillStateInitialized = false;

	Super::EndPlay(EndPlayReason);
}

bool UMPlayerSkillComponent::EnsureSkillStateInitialized() const
{
	if (bSkillStateInitialized)
	{
		return true;
	}

	if (!bSkillStateLoading)
	{
		UMPlayerSkillComponent* MutableThis = const_cast<UMPlayerSkillComponent*>(this);
		MutableThis->InitializeSkillState();
	}

	return false;
}

void UMPlayerSkillComponent::InitializeSkillState()
{
	if (bSkillStateInitialized || bSkillStateLoading)
	{
		return;
	}

	SkillInstances.Reset();
	EquippedSkillTagsBySlot.Reset();
	EquippedSkillHandlesBySlot.Reset();
	LoadedSkillTreeDefinition = nullptr;
	bSkillStateInitialized = false;
	bSkillStateLoading = true;

	UMDefinitionSubsystem* DefinitionSubsystem = ResolveDefinitionSubsystem();
	if (!DefinitionSubsystem || !SkillTreeTag.IsValid())
	{
		bSkillStateLoading = false;
		return;
	}

	if (!DefinitionSubsystem->LoadCollectionByTagAsync(
		SkillTreeTag,
		FOnDefinitionCollectionLoaded::CreateUObject(this, &ThisClass::HandleSkillTreeLoaded)))
	{
		bSkillStateLoading = false;
	}
}

void UMPlayerSkillComponent::HandleSkillTreeLoaded(UMDefinitionCollectionDataAsset* LoadedCollection)
{
	bSkillStateLoading = false;
	SkillInstances.Reset();
	EquippedSkillTagsBySlot.Reset();
	EquippedSkillHandlesBySlot.Reset();
	LoadedSkillTreeDefinition = Cast<UMSkillTreeDataAsset>(LoadedCollection);
	bSkillStateInitialized = LoadedSkillTreeDefinition != nullptr;

	if (!LoadedSkillTreeDefinition)
	{
		return;
	}

	const TArray<TObjectPtr<UMSkillDefinition>>& SkillDefinitions = LoadedSkillTreeDefinition->GetSkillDefinitions();
	SkillInstances.Reserve(SkillDefinitions.Num());

	for (UMSkillDefinition* SkillDefinition : SkillDefinitions)
	{
		if (!SkillDefinition)
		{
			continue;
		}

		const FGameplayTag SkillTag = SkillDefinition->GetSkillTag();
		if (!SkillTag.IsValid() || SkillInstances.Contains(SkillTag))
		{
			continue;
		}

		UMSkillInstance* SkillInstance = NewObject<UMSkillInstance>(this);
		if (!SkillInstance || !SkillInstance->InitializeSkillInstance(SkillDefinition))
		{
			continue;
		}

		SkillInstances.Add(SkillTag, SkillInstance);
	}

	BroadcastSkillStateChanged({});
}

void UMPlayerSkillComponent::BroadcastSkillStateChanged(const TArray<FGameplayTag>& ChangedSlotTags)
{
	OnSkillStateChanged.Broadcast();
	OnSkillLoadoutChanged.Broadcast();

	for (const FGameplayTag& SlotTag : ChangedSlotTags)
	{
		OnSkillSlotChanged.Broadcast(SlotTag, GetAssignedSkillTag(SlotTag));
	}
}

UAbilitySystemComponent* UMPlayerSkillComponent::ResolveAbilitySystemComponent() const
{
	const AMAbilityCharacterBase* AbilityCharacter = Cast<AMAbilityCharacterBase>(GetOwner());
	return AbilityCharacter ? AbilityCharacter->GetAbilitySystemComponent() : nullptr;
}

UMDefinitionSubsystem* UMPlayerSkillComponent::ResolveDefinitionSubsystem() const
{
	UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	return GameInstance ? GameInstance->GetSubsystem<UMDefinitionSubsystem>() : nullptr;
}

const UMSkillTreeDataAsset* UMPlayerSkillComponent::ResolveSkillTreeDefinition() const
{
	return LoadedSkillTreeDefinition.Get();
}

const UMSkillDefinition* UMPlayerSkillComponent::ResolveSkillDefinition(const FGameplayTag& SkillTag) const
{
	const UMSkillTreeDataAsset* SkillTreeDefinition = ResolveSkillTreeDefinition();
	return SkillTreeDefinition ? SkillTreeDefinition->FindSkillDefinitionByTag(SkillTag) : nullptr;
}

bool UMPlayerSkillComponent::ClearEquippedSlotInternal(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& SlotTag, FGameplayTag* OutRemovedSkillTag)
{
	if (!AbilitySystemComponent || !SlotTag.IsValid())
	{
		return false;
	}

	bool bRemoved = false;

	if (FGameplayAbilitySpecHandle* ExistingHandle = EquippedSkillHandlesBySlot.Find(SlotTag))
	{
		if (ExistingHandle->IsValid())
		{
			AbilitySystemComponent->ClearAbility(*ExistingHandle);
		}

		EquippedSkillHandlesBySlot.Remove(SlotTag);
		bRemoved = true;
	}

	if (const FGameplayTag* EquippedSkillTag = EquippedSkillTagsBySlot.Find(SlotTag))
	{
		if (OutRemovedSkillTag)
		{
			*OutRemovedSkillTag = *EquippedSkillTag;
		}

		EquippedSkillTagsBySlot.Remove(SlotTag);
		bRemoved = true;
	}

	return bRemoved;
}

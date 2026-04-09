#include "Gameplay/Character/Player/Component/MPlayerInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "Engine/GameInstance.h"
#include "Gameplay/Character/MAbilityCharacterBase.h"
#include "Gameplay/Definition/MDefinitionSubsystem.h"
#include "Gameplay/Item/MItemDefinition.h"

UMPlayerInventoryComponent::UMPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMPlayerInventoryComponent::GetInventoryItems(TArray<UMItemInstance*>& OutInventoryItems) const
{
	OutInventoryItems.Reset(InventoryItems.Num());

	for (const TObjectPtr<UMItemInstance>& InventoryItem : InventoryItems)
	{
		if (InventoryItem)
		{
			OutInventoryItems.Add(InventoryItem.Get());
		}
	}

	return OutInventoryItems.Num() > 0;
}

bool UMPlayerInventoryComponent::GetEquippedItems(TArray<UMItemInstance*>& OutEquippedItems) const
{
	OutEquippedItems.Reset(EquippedItems.Num());

	for (const TObjectPtr<UMItemInstance>& EquippedItem : EquippedItems)
	{
		if (EquippedItem)
		{
			OutEquippedItems.Add(EquippedItem.Get());
		}
	}

	return OutEquippedItems.Num() > 0;
}

UMItemInstance* UMPlayerInventoryComponent::FindInventoryItemByTag(const FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid())
	{
		return nullptr;
	}

	for (const TObjectPtr<UMItemInstance>& InventoryItem : InventoryItems)
	{
		if (InventoryItem && InventoryItem->GetItemTag().MatchesTagExact(ItemTag))
		{
			return InventoryItem.Get();
		}
	}

	return nullptr;
}

UMItemInstance* UMPlayerInventoryComponent::GetEquippedItemBySlot(const FGameplayTag EquipSlotTag) const
{
	if (!EquipSlotTag.IsValid())
	{
		return nullptr;
	}

	if (const TObjectPtr<UMItemInstance>* EquippedItem = EquippedItemsBySlot.Find(EquipSlotTag))
	{
		return EquippedItem->Get();
	}

	return nullptr;
}

UMItemInstance* UMPlayerInventoryComponent::GetAssignedConsumableItem(const FGameplayTag QuickSlotTag) const
{
	if (!QuickSlotTag.IsValid())
	{
		return nullptr;
	}

	if (const TObjectPtr<UMItemInstance>* AssignedItem = AssignedConsumableItemsBySlot.Find(QuickSlotTag))
	{
		return AssignedItem->Get();
	}

	return nullptr;
}

FGameplayTag UMPlayerInventoryComponent::GetAssignedConsumableTag(const FGameplayTag QuickSlotTag) const
{
	const UMItemInstance* AssignedItem = GetAssignedConsumableItem(QuickSlotTag);
	return AssignedItem ? AssignedItem->GetItemTag() : FGameplayTag();
}

bool UMPlayerInventoryComponent::AddItemByTag(const FGameplayTag ItemTag, const int32 Quantity)
{
	UMItemDefinition* ItemDefinition = ResolveItemDefinition(ItemTag);
	return AddItemDefinition(ItemDefinition, Quantity);
}

bool UMPlayerInventoryComponent::AddItemDefinition(UMItemDefinition* ItemDefinition, int32 Quantity)
{
	if (!ItemDefinition || Quantity <= 0)
	{
		return false;
	}

	bool bAddedAnyItem = false;
	const int32 MaxStackCount = FMath::Max(1, ItemDefinition->GetMaxStackCount());
	const FGameplayTag ItemTag = ItemDefinition->GetItemTag();

	if (ItemDefinition->IsStackable())
	{
		for (const TObjectPtr<UMItemInstance>& InventoryItem : InventoryItems)
		{
			if (Quantity <= 0)
			{
				break;
			}

			if (!InventoryItem || !InventoryItem->GetItemTag().MatchesTagExact(ItemTag) || !InventoryItem->IsStackable())
			{
				continue;
			}

			const int32 CurrentQuantity = InventoryItem->GetQuantity();
			const int32 AvailableSpace = FMath::Max(0, MaxStackCount - CurrentQuantity);
			if (AvailableSpace <= 0)
			{
				continue;
			}

			const int32 AddedQuantity = FMath::Min(Quantity, AvailableSpace);
			InventoryItem->SetQuantity(CurrentQuantity + AddedQuantity);
			Quantity -= AddedQuantity;
			bAddedAnyItem = true;
		}
	}

	while (Quantity > 0)
	{
		const int32 StackQuantity = ItemDefinition->IsStackable() ? FMath::Min(Quantity, MaxStackCount) : 1;
		UMItemInstance* NewItemInstance = CreateItemInstance(ItemDefinition, StackQuantity);
		if (!NewItemInstance)
		{
			break;
		}

		InventoryItems.Add(NewItemInstance);
		Quantity -= StackQuantity;
		bAddedAnyItem = true;
	}

	if (bAddedAnyItem)
	{
		BroadcastInventoryStateChanged();
	}

	return bAddedAnyItem;
}

bool UMPlayerInventoryComponent::RemoveInventoryItem(UMItemInstance* ItemInstance, const int32 Quantity)
{
	if (!ItemInstance || Quantity <= 0 || !IsInventoryItemInstance(ItemInstance))
	{
		return false;
	}

	const int32 CurrentQuantity = ItemInstance->GetQuantity();
	const int32 RemainingQuantity = FMath::Max(0, CurrentQuantity - Quantity);

	if (RemainingQuantity > 0)
	{
		ItemInstance->SetQuantity(RemainingQuantity);
		BroadcastInventoryStateChanged();
		return true;
	}

	ClearConsumableAssignmentsForItem(ItemInstance);
	InventoryItems.RemoveSingle(ItemInstance);
	BroadcastInventoryStateChanged();
	return true;
}

bool UMPlayerInventoryComponent::EquipItem(UMItemInstance* ItemInstance)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !ItemInstance || !IsInventoryItemInstance(ItemInstance))
	{
		return false;
	}

	UMItemDefinitionEquipment* EquipmentDefinition = ItemInstance->GetEquipmentDefinition();
	if (!EquipmentDefinition)
	{
		return false;
	}

	const FGameplayTag EquipSlotTag = EquipmentDefinition->GetEquipSlotTag();
	if (!EquipSlotTag.IsValid())
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	if (!EquipmentDefinition->GetEquipGameplayEffectClass().IsNull() && !AbilitySystemComponent)
	{
		return false;
	}

	UMItemInstance* PreviouslyEquippedItem = nullptr;
	if (ClearEquippedSlotInternal(AbilitySystemComponent, EquipSlotTag, &PreviouslyEquippedItem) && PreviouslyEquippedItem)
	{
		InventoryItems.Add(PreviouslyEquippedItem);
	}

	InventoryItems.RemoveSingle(ItemInstance);
	EquippedItems.RemoveSingle(ItemInstance);
	EquippedItems.Add(ItemInstance);
	EquippedItemsBySlot.FindOrAdd(EquipSlotTag) = ItemInstance;

	FActiveGameplayEffectHandle EquippedEffectHandle;
	if (!ApplyEquipmentEffect(AbilitySystemComponent, ItemInstance, EquippedEffectHandle))
	{
		EquippedItemsBySlot.Remove(EquipSlotTag);
		EquippedItems.RemoveSingle(ItemInstance);
		InventoryItems.Add(ItemInstance);
		return false;
	}

	if (EquippedEffectHandle.IsValid())
	{
		EquippedEffectHandlesBySlot.FindOrAdd(EquipSlotTag) = EquippedEffectHandle;
	}
	else
	{
		EquippedEffectHandlesBySlot.Remove(EquipSlotTag);
	}

	ItemInstance->SetEquippedSlotTag(EquipSlotTag);
	BroadcastInventoryStateChanged();
	BroadcastEquipmentSlotChanged(EquipSlotTag);
	return true;
}

bool UMPlayerInventoryComponent::UnequipItem(const FGameplayTag EquipSlotTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !EquipSlotTag.IsValid())
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	UMItemInstance* RemovedItem = nullptr;
	if (!ClearEquippedSlotInternal(AbilitySystemComponent, EquipSlotTag, &RemovedItem) || !RemovedItem)
	{
		return false;
	}

	InventoryItems.Add(RemovedItem);
	BroadcastInventoryStateChanged();
	BroadcastEquipmentSlotChanged(EquipSlotTag);
	return true;
}

bool UMPlayerInventoryComponent::AssignConsumableToSlot(UMItemInstance* ItemInstance, const FGameplayTag QuickSlotTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !QuickSlotTag.IsValid() || !ItemInstance || !IsInventoryItemInstance(ItemInstance))
	{
		return false;
	}

	if (!ItemInstance->IsConsumableItem() || ItemInstance->GetQuantity() <= 0)
	{
		return false;
	}

	UMItemInstance* PreviousItemInSlot = nullptr;
	ClearConsumableSlotInternal(QuickSlotTag, &PreviousItemInSlot);

	const FGameplayTag PreviousQuickSlotTag = ItemInstance->GetAssignedQuickSlotTag();
	if (PreviousQuickSlotTag.IsValid() && PreviousQuickSlotTag != QuickSlotTag)
	{
		ClearConsumableSlotInternal(PreviousQuickSlotTag);
	}

	AssignedConsumableItemsBySlot.FindOrAdd(QuickSlotTag) = ItemInstance;
	ItemInstance->SetAssignedQuickSlotTag(QuickSlotTag);
	BroadcastInventoryStateChanged();
	BroadcastConsumableSlotChanged(QuickSlotTag);
	return true;
}

bool UMPlayerInventoryComponent::ClearConsumableSlot(const FGameplayTag QuickSlotTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !QuickSlotTag.IsValid())
	{
		return false;
	}

	UMItemInstance* RemovedItem = nullptr;
	if (!ClearConsumableSlotInternal(QuickSlotTag, &RemovedItem))
	{
		return false;
	}

	BroadcastInventoryStateChanged();
	BroadcastConsumableSlotChanged(QuickSlotTag);
	return true;
}

bool UMPlayerInventoryComponent::ConsumeAssignedItem(const FGameplayTag QuickSlotTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !QuickSlotTag.IsValid())
	{
		return false;
	}

	UMItemInstance* ItemInstance = GetAssignedConsumableItem(QuickSlotTag);
	UMItemDefinitionConsumable* ConsumableDefinition = ItemInstance ? ItemInstance->GetConsumableDefinition() : nullptr;
	if (!ItemInstance || !ConsumableDefinition || ItemInstance->GetQuantity() <= 0)
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	const UGameplayEffect* ConsumeGameplayEffectClass = Cast<UGameplayEffect>(ConsumableDefinition->GetConsumeGameplayEffectClass().LoadSynchronous());
	if (!ConsumeGameplayEffectClass)
	{
		return false;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(ItemInstance);
	AbilitySystemComponent->ApplyGameplayEffectToSelf(ConsumeGameplayEffectClass, 1.0f, EffectContext);

	const int32 ConsumeCount = FMath::Max(1, ConsumableDefinition->GetConsumeCountPerUse());
	return RemoveInventoryItem(ItemInstance, ConsumeCount);
}

void UMPlayerInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearEquipmentState();
	OnInventoryStateChanged.Clear();
	OnEquipmentSlotChanged.Clear();
	OnConsumableSlotChanged.Clear();
	AssignedConsumableItemsBySlot.Reset();
	InventoryItems.Reset();
	EquippedItems.Reset();
	EquippedItemsBySlot.Reset();

	Super::EndPlay(EndPlayReason);
}

UMItemDefinition* UMPlayerInventoryComponent::ResolveItemDefinition(const FGameplayTag ItemTag) const
{
	UMDefinitionSubsystem* DefinitionSubsystem = ResolveDefinitionSubsystem();
	return DefinitionSubsystem ? DefinitionSubsystem->GetDefinition<UMItemDefinition>(ItemTag) : nullptr;
}

UMDefinitionSubsystem* UMPlayerInventoryComponent::ResolveDefinitionSubsystem() const
{
	UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	return GameInstance ? GameInstance->GetSubsystem<UMDefinitionSubsystem>() : nullptr;
}

UAbilitySystemComponent* UMPlayerInventoryComponent::ResolveAbilitySystemComponent() const
{
	const AMAbilityCharacterBase* AbilityCharacter = Cast<AMAbilityCharacterBase>(GetOwner());
	return AbilityCharacter ? AbilityCharacter->GetAbilitySystemComponent() : nullptr;
}

UMItemInstance* UMPlayerInventoryComponent::CreateItemInstance(UMItemDefinition* ItemDefinition, const int32 Quantity)
{
	if (!ItemDefinition || Quantity <= 0)
	{
		return nullptr;
	}

	UMItemInstance* ItemInstance = NewObject<UMItemInstance>(this);
	return ItemInstance && ItemInstance->InitializeItemInstance(ItemDefinition, Quantity) ? ItemInstance : nullptr;
}

bool UMPlayerInventoryComponent::IsInventoryItemInstance(const UMItemInstance* ItemInstance) const
{
	return ItemInstance && InventoryItems.Contains(ItemInstance);
}

bool UMPlayerInventoryComponent::ClearEquippedSlotInternal(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag EquipSlotTag, UMItemInstance** OutRemovedItem)
{
	if (OutRemovedItem)
	{
		*OutRemovedItem = nullptr;
	}

	if (!EquipSlotTag.IsValid())
	{
		return false;
	}

	bool bRemovedAnyState = false;

	if (TObjectPtr<UMItemInstance>* EquippedItem = EquippedItemsBySlot.Find(EquipSlotTag))
	{
		if (OutRemovedItem)
		{
			*OutRemovedItem = EquippedItem->Get();
		}

		if (EquippedItem->Get())
		{
			EquippedItem->Get()->SetEquippedSlotTag(FGameplayTag());
			EquippedItems.RemoveSingle(EquippedItem->Get());
		}

		EquippedItemsBySlot.Remove(EquipSlotTag);
		bRemovedAnyState = true;
	}

	RemoveEquipmentEffect(AbilitySystemComponent, EquipSlotTag);
	return bRemovedAnyState;
}

bool UMPlayerInventoryComponent::ClearConsumableSlotInternal(const FGameplayTag QuickSlotTag, UMItemInstance** OutRemovedItem)
{
	if (OutRemovedItem)
	{
		*OutRemovedItem = nullptr;
	}

	if (!QuickSlotTag.IsValid())
	{
		return false;
	}

	TObjectPtr<UMItemInstance>* AssignedItem = AssignedConsumableItemsBySlot.Find(QuickSlotTag);
	if (!AssignedItem)
	{
		return false;
	}

	if (OutRemovedItem)
	{
		*OutRemovedItem = AssignedItem->Get();
	}

	if (AssignedItem->Get())
	{
		AssignedItem->Get()->SetAssignedQuickSlotTag(FGameplayTag());
	}

	AssignedConsumableItemsBySlot.Remove(QuickSlotTag);
	return true;
}

bool UMPlayerInventoryComponent::ApplyEquipmentEffect(UAbilitySystemComponent* AbilitySystemComponent, UMItemInstance* ItemInstance, FActiveGameplayEffectHandle& OutHandle) const
{
	OutHandle = FActiveGameplayEffectHandle();

	const UMItemDefinitionEquipment* EquipmentDefinition = ItemInstance ? ItemInstance->GetEquipmentDefinition() : nullptr;
	if (!EquipmentDefinition)
	{
		return false;
	}

	const TSoftClassPtr<UGameplayEffect>& EffectClassPtr = EquipmentDefinition->GetEquipGameplayEffectClass();
	if (EffectClassPtr.IsNull())
	{
		return true;
	}

	if (!AbilitySystemComponent)
	{
		return false;
	}

	const UGameplayEffect* EffectClass = Cast<UGameplayEffect>(EffectClassPtr.LoadSynchronous());
	if (!EffectClass)
	{
		return false;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(ItemInstance);
	OutHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectClass, 1.0f, EffectContext);
	return true;
}

void UMPlayerInventoryComponent::RemoveEquipmentEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag EquipSlotTag)
{
	if (!EquipSlotTag.IsValid())
	{
		return;
	}

	FActiveGameplayEffectHandle RemovedHandle;
	if (!EquippedEffectHandlesBySlot.RemoveAndCopyValue(EquipSlotTag, RemovedHandle))
	{
		return;
	}

	if (AbilitySystemComponent && RemovedHandle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(RemovedHandle);
	}
}

void UMPlayerInventoryComponent::BroadcastInventoryStateChanged()
{
	OnInventoryStateChanged.Broadcast();
}

void UMPlayerInventoryComponent::BroadcastEquipmentSlotChanged(const FGameplayTag EquipSlotTag)
{
	if (!EquipSlotTag.IsValid())
	{
		return;
	}

	OnEquipmentSlotChanged.Broadcast(EquipSlotTag, GetEquippedItemBySlot(EquipSlotTag));
}

void UMPlayerInventoryComponent::BroadcastConsumableSlotChanged(const FGameplayTag QuickSlotTag)
{
	if (!QuickSlotTag.IsValid())
	{
		return;
	}

	OnConsumableSlotChanged.Broadcast(QuickSlotTag, GetAssignedConsumableItem(QuickSlotTag));
}

void UMPlayerInventoryComponent::ClearConsumableAssignmentsForItem(UMItemInstance* ItemInstance)
{
	if (!ItemInstance)
	{
		return;
	}

	TArray<FGameplayTag> QuickSlotTagsToClear;
	for (const TPair<FGameplayTag, TObjectPtr<UMItemInstance>>& Pair : AssignedConsumableItemsBySlot)
	{
		if (Pair.Value == ItemInstance)
		{
			QuickSlotTagsToClear.Add(Pair.Key);
		}
	}

	for (const FGameplayTag& QuickSlotTag : QuickSlotTagsToClear)
	{
		if (ClearConsumableSlotInternal(QuickSlotTag))
		{
			BroadcastConsumableSlotChanged(QuickSlotTag);
		}
	}
}

void UMPlayerInventoryComponent::ClearEquipmentState()
{
	UAbilitySystemComponent* AbilitySystemComponent = ResolveAbilitySystemComponent();
	TArray<FGameplayTag> EquippedSlotTags;
	EquippedItemsBySlot.GetKeys(EquippedSlotTags);

	for (const FGameplayTag& EquipSlotTag : EquippedSlotTags)
	{
		ClearEquippedSlotInternal(AbilitySystemComponent, EquipSlotTag);
	}
}

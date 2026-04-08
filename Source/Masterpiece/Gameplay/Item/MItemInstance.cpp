#include "Gameplay/Item/MItemInstance.h"

bool UMItemInstance::InitializeItemInstance(UMItemDefinition* InDefinition, const int32 InQuantity)
{
	if (!InDefinition)
	{
		return false;
	}

	InitializeDefinition(InDefinition);
	Quantity = FMath::Clamp(InQuantity, 1, FMath::Max(1, InDefinition->GetMaxStackCount()));
	RefreshItemViewData();
	return true;
}

UMItemDefinition* UMItemInstance::GetItemDefinition() const
{
	return Cast<UMItemDefinition>(Definition);
}

UMItemDefinitionEquipment* UMItemInstance::GetEquipmentDefinition() const
{
	return Cast<UMItemDefinitionEquipment>(Definition);
}

UMItemDefinitionConsumable* UMItemInstance::GetConsumableDefinition() const
{
	return Cast<UMItemDefinitionConsumable>(Definition);
}

UMItemDefinitionQuest* UMItemInstance::GetQuestDefinition() const
{
	return Cast<UMItemDefinitionQuest>(Definition);
}

UMItemDefinitionMisc* UMItemInstance::GetMiscDefinition() const
{
	return Cast<UMItemDefinitionMisc>(Definition);
}

FGameplayTag UMItemInstance::GetItemTag() const
{
	return ItemTag;
}

bool UMItemInstance::MatchesItemTag(const FGameplayTag InItemTag) const
{
	return ItemTag.IsValid() && InItemTag.IsValid() && ItemTag.MatchesTag(InItemTag);
}

TSoftObjectPtr<UTexture2D> UMItemInstance::GetIcon() const
{
	return Icon;
}

FText UMItemInstance::GetDisplayName() const
{
	return DisplayName;
}

FText UMItemInstance::GetDescription() const
{
	return Description;
}

bool UMItemInstance::IsEquipmentItem() const
{
	const UMItemDefinition* ItemDefinition = GetItemDefinitionChecked();
	return ItemDefinition && ItemDefinition->IsEquipmentDefinition();
}

bool UMItemInstance::IsConsumableItem() const
{
	const UMItemDefinition* ItemDefinition = GetItemDefinitionChecked();
	return ItemDefinition && ItemDefinition->IsConsumableDefinition();
}

bool UMItemInstance::IsQuestItem() const
{
	const UMItemDefinition* ItemDefinition = GetItemDefinitionChecked();
	return ItemDefinition && ItemDefinition->IsQuestDefinition();
}

bool UMItemInstance::IsMiscItem() const
{
	const UMItemDefinition* ItemDefinition = GetItemDefinitionChecked();
	return ItemDefinition && ItemDefinition->IsMiscDefinition();
}

bool UMItemInstance::IsStackable() const
{
	return bStackable;
}

int32 UMItemInstance::GetMaxStackCount() const
{
	return MaxStackCount;
}

int32 UMItemInstance::GetBuyPrice() const
{
	return BuyPrice;
}

int32 UMItemInstance::GetSellPrice() const
{
	return SellPrice;
}

int32 UMItemInstance::GetQuantity() const
{
	return Quantity;
}

void UMItemInstance::SetQuantity(const int32 InQuantity)
{
	const int32 NewQuantity = FMath::Clamp(InQuantity, 0, FMath::Max(1, GetMaxStackCount()));
	if (Quantity == NewQuantity)
	{
		return;
	}

	Quantity = NewQuantity;
	RefreshItemViewData();
}

const UMItemDefinition* UMItemInstance::GetItemDefinitionChecked() const
{
	return Cast<UMItemDefinition>(Definition);
}

void UMItemInstance::RefreshItemViewData()
{
	const UMItemDefinition* ItemDefinition = GetItemDefinitionChecked();

	ItemTag = ItemDefinition ? ItemDefinition->GetItemTag() : FGameplayTag();
	Icon = ItemDefinition ? ItemDefinition->GetIcon() : TSoftObjectPtr<UTexture2D>();
	DisplayName = ItemDefinition ? ItemDefinition->GetDisplayName() : FText::GetEmpty();
	Description = ItemDefinition ? ItemDefinition->GetDescription() : FText::GetEmpty();
	bStackable = ItemDefinition && ItemDefinition->IsStackable();
	MaxStackCount = ItemDefinition ? ItemDefinition->GetMaxStackCount() : 1;
	BuyPrice = ItemDefinition ? ItemDefinition->GetBuyPrice() : 0;
	SellPrice = ItemDefinition ? ItemDefinition->GetSellPrice() : 0;
	Quantity = FMath::Clamp(Quantity, 0, FMath::Max(1, MaxStackCount));

	BroadcastDefinitionInstanceChanged();
}

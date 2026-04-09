#include "Gameplay/UI/Inventory/MInventoryEquipmentSlotWidget.h"

#include "Components/TextBlock.h"
#include "Gameplay/Character/Player/Component/MPlayerInventoryComponent.h"
#include "Gameplay/Definition/MDefinitionInstance.h"
#include "Gameplay/Item/MItemDefinition.h"
#include "Gameplay/Item/MItemInstance.h"

void UMInventoryEquipmentSlotWidget::InitializeEquipmentSlot(UMPlayerInventoryComponent* InInventoryComponent, const FGameplayTag InEquipSlotTag)
{
	BoundInventoryComponent = InInventoryComponent;
	EquipSlotTag = InEquipSlotTag;

	K2_OnEquipmentSlotInitialized(BoundInventoryComponent.Get(), EquipSlotTag);
	RefreshWidget();
}

UMPlayerInventoryComponent* UMInventoryEquipmentSlotWidget::GetInventoryComponent() const
{
	return BoundInventoryComponent.Get();
}

FGameplayTag UMInventoryEquipmentSlotWidget::GetEquipSlotTag() const
{
	return EquipSlotTag;
}

void UMInventoryEquipmentSlotWidget::NativeRefreshWidget()
{
	Super::NativeRefreshWidget();

	if (EquipSlotName)
	{
		EquipSlotName->SetText(BuildEquipSlotNameText());
	}
}

bool UMInventoryEquipmentSlotWidget::CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const
{
	const UMItemInstance* ItemInstance = Cast<UMItemInstance>(DefinitionInstance);
	const UMItemDefinitionEquipment* EquipmentDefinition = ItemInstance ? ItemInstance->GetEquipmentDefinition() : nullptr;
	return BoundInventoryComponent
		&& EquipSlotTag.IsValid()
		&& EquipmentDefinition
		&& EquipmentDefinition->GetEquipSlotTag().MatchesTagExact(EquipSlotTag);
}

bool UMInventoryEquipmentSlotWidget::DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance)
{
	UMItemInstance* ItemInstance = Cast<UMItemInstance>(DefinitionInstance);
	return BoundInventoryComponent && ItemInstance ? BoundInventoryComponent->EquipItem(ItemInstance) : false;
}

FText UMInventoryEquipmentSlotWidget::BuildEquipSlotNameText() const
{
	const UMItemInstance* ItemInstance = GetItemInstance();
	if (ItemInstance)
	{
		return ItemInstance->GetDisplayName();
	}

	return EquipSlotTag.IsValid()
		? FText::FromString(EquipSlotTag.GetTagName().ToString())
		: FText::GetEmpty();
}

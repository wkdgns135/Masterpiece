#include "Gameplay/UI/Inventory/MInventoryItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Gameplay/Item/MItemInstance.h"
#include "Gameplay/UI/Item/MItemTooltipWidget.h"

UMInventoryItemSlotWidget::UMInventoryItemSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefinitionToolTipWidgetClass = UMItemTooltipWidget::StaticClass();
}

void UMInventoryItemSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	RefreshWidget();
}

void UMInventoryItemSlotWidget::NativeRefreshWidget()
{
	const UMItemInstance* ItemInstance = GetItemInstance();
	if (!ItemInstance)
	{
		if (ItemIcon)
		{
			ItemIcon->SetBrushFromTexture(nullptr);
		}
		if (ItemQuantity)
		{
			ItemQuantity->SetText(FText::GetEmpty());
		}
		if (ItemName)
		{
			ItemName->SetText(FText::GetEmpty());
		}
		return;
	}

	if (ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(ItemInstance->GetIcon().LoadSynchronous());
	}
	if (ItemQuantity)
	{
		ItemQuantity->SetText(
			ItemInstance->GetQuantity() > 1
				? FText::AsNumber(ItemInstance->GetQuantity())
				: FText::GetEmpty());
	}
	if (ItemName)
	{
		ItemName->SetText(ItemInstance->GetDisplayName());
	}
}

UImage* UMInventoryItemSlotWidget::GetDragHandleImage_Implementation() const
{
	return ItemIcon.Get();
}

bool UMInventoryItemSlotWidget::CanDragDefinitionInstance_Implementation() const
{
	const UMItemInstance* ItemInstance = GetItemInstance();
	return ItemInstance && ItemInstance->GetQuantity() > 0;
}

UMItemInstance* UMInventoryItemSlotWidget::GetItemInstance() const
{
	return Cast<UMItemInstance>(GetDefinitionInstance());
}

#include "Gameplay/UI/Item/MItemTooltipWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Gameplay/Item/MItemInstance.h"

#define LOCTEXT_NAMESPACE "MItemTooltipWidget"

UMItemInstance* UMItemTooltipWidget::GetItemInstance() const
{
	return Cast<UMItemInstance>(GetDefinitionInstance());
}

void UMItemTooltipWidget::NativeConstruct()
{
	EnsureDefaultLayout();

	Super::NativeConstruct();

	EnsureDefaultLayout();
	RefreshItemTooltip();
}

void UMItemTooltipWidget::HandleDefinitionInstanceSet()
{
	EnsureDefaultLayout();
	RefreshItemTooltip();
	K2_OnItemInstanceSet(GetItemInstance());
}

void UMItemTooltipWidget::EnsureDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UBorder* RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("RootBorder"));
	USizeBox* RootSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("RootSizeBox"));
	UVerticalBox* RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootBox"));
	UHorizontalBox* HeaderBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HeaderBox"));
	UVerticalBox* HeaderTextBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HeaderTextBox"));

	ItemIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("ItemIcon"));
	ItemName = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemName"));
	ItemType = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemType"));
	ItemDescription = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemDescription"));
	ItemQuantity = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemQuantity"));
	ItemValue = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemValue"));
	ItemState = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ItemState"));

	WidgetTree->RootWidget = RootBorder;

	RootBorder->SetBrushColor(FLinearColor(0.07f, 0.05f, 0.03f, 0.97f));
	RootBorder->SetPadding(FMargin(16.0f));
	RootBorder->SetContent(RootSizeBox);

	RootSizeBox->SetWidthOverride(320.0f);
	RootSizeBox->SetContent(RootBox);

	if (UVerticalBoxSlot* HeaderSlot = RootBox->AddChildToVerticalBox(HeaderBox))
	{
		HeaderSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	}

	ItemIcon->SetBrushSize(FVector2D(48.0f, 48.0f));
	if (UHorizontalBoxSlot* IconSlot = HeaderBox->AddChildToHorizontalBox(ItemIcon))
	{
		IconSlot->SetPadding(FMargin(0.0f, 0.0f, 12.0f, 0.0f));
		IconSlot->SetVerticalAlignment(VAlign_Top);
	}

	if (UHorizontalBoxSlot* HeaderTextSlot = HeaderBox->AddChildToHorizontalBox(HeaderTextBox))
	{
		HeaderTextSlot->SetHorizontalAlignment(HAlign_Fill);
		HeaderTextSlot->SetVerticalAlignment(VAlign_Center);
		HeaderTextSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	ItemName->SetColorAndOpacity(FSlateColor(FLinearColor(0.98f, 0.95f, 0.88f, 1.0f)));
	ItemName->SetAutoWrapText(true);
	if (UVerticalBoxSlot* NameSlot = HeaderTextBox->AddChildToVerticalBox(ItemName))
	{
		NameSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
	}

	ItemType->SetColorAndOpacity(FSlateColor(FLinearColor(0.88f, 0.76f, 0.50f, 1.0f)));
	if (UVerticalBoxSlot* TypeSlot = HeaderTextBox->AddChildToVerticalBox(ItemType))
	{
		TypeSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
	}

	ItemState->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.85f, 0.93f, 1.0f)));
	HeaderTextBox->AddChildToVerticalBox(ItemState);

	ItemDescription->SetColorAndOpacity(FSlateColor(FLinearColor(0.92f, 0.90f, 0.86f, 1.0f)));
	ItemDescription->SetAutoWrapText(true);
	if (UVerticalBoxSlot* DescriptionSlot = RootBox->AddChildToVerticalBox(ItemDescription))
	{
		DescriptionSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	}

	ItemQuantity->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.86f, 0.90f, 1.0f)));
	if (UVerticalBoxSlot* QuantitySlot = RootBox->AddChildToVerticalBox(ItemQuantity))
	{
		QuantitySlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
	}

	ItemValue->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.86f, 0.90f, 1.0f)));
	RootBox->AddChildToVerticalBox(ItemValue);
}

void UMItemTooltipWidget::RefreshItemTooltip()
{
	const UMItemInstance* ItemInstance = GetItemInstance();
	if (!ItemInstance)
	{
		if (ItemIcon)
		{
			ItemIcon->SetBrushFromTexture(nullptr);
		}
		if (ItemName)
		{
			ItemName->SetText(FText::GetEmpty());
		}
		if (ItemType)
		{
			ItemType->SetText(FText::GetEmpty());
		}
		if (ItemDescription)
		{
			ItemDescription->SetText(FText::GetEmpty());
		}
		if (ItemQuantity)
		{
			ItemQuantity->SetText(FText::GetEmpty());
		}
		if (ItemValue)
		{
			ItemValue->SetText(FText::GetEmpty());
		}
		if (ItemState)
		{
			ItemState->SetText(FText::GetEmpty());
		}
		return;
	}

	if (ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(ItemInstance->GetIcon().LoadSynchronous());
	}
	if (ItemName)
	{
		ItemName->SetText(ItemInstance->GetDisplayName());
	}
	if (ItemType)
	{
		ItemType->SetText(BuildTypeText(ItemInstance));
	}
	if (ItemDescription)
	{
		ItemDescription->SetText(ItemInstance->GetDescription());
	}
	if (ItemQuantity)
	{
		ItemQuantity->SetText(BuildQuantityText(ItemInstance));
	}
	if (ItemValue)
	{
		ItemValue->SetText(BuildValueText(ItemInstance));
	}
	if (ItemState)
	{
		ItemState->SetText(BuildStateText(ItemInstance));
	}
}

FText UMItemTooltipWidget::BuildTypeText(const UMItemInstance* InItemInstance) const
{
	if (!InItemInstance)
	{
		return FText::GetEmpty();
	}

	if (InItemInstance->IsEquipmentItem())
	{
		return LOCTEXT("ItemTypeEquipment", "Equipment");
	}
	if (InItemInstance->IsConsumableItem())
	{
		return LOCTEXT("ItemTypeConsumable", "Consumable");
	}
	if (InItemInstance->IsQuestItem())
	{
		return LOCTEXT("ItemTypeQuest", "Quest");
	}

	return LOCTEXT("ItemTypeMisc", "Misc");
}

FText UMItemTooltipWidget::BuildQuantityText(const UMItemInstance* InItemInstance) const
{
	return InItemInstance
		? FText::Format(
			LOCTEXT("ItemQuantityFormat", "Quantity {0}/{1}"),
			FText::AsNumber(InItemInstance->GetQuantity()),
			FText::AsNumber(InItemInstance->GetMaxStackCount()))
		: FText::GetEmpty();
}

FText UMItemTooltipWidget::BuildValueText(const UMItemInstance* InItemInstance) const
{
	return InItemInstance
		? FText::Format(
			LOCTEXT("ItemValueFormat", "Buy {0} / Sell {1}"),
			FText::AsNumber(InItemInstance->GetBuyPrice()),
			FText::AsNumber(InItemInstance->GetSellPrice()))
		: FText::GetEmpty();
}

FText UMItemTooltipWidget::BuildStateText(const UMItemInstance* InItemInstance) const
{
	if (!InItemInstance)
	{
		return FText::GetEmpty();
	}

	if (InItemInstance->IsEquipped())
	{
		return LOCTEXT("ItemStateEquipped", "Equipped");
	}
	if (InItemInstance->IsAssignedToQuickSlot())
	{
		return LOCTEXT("ItemStateQuickSlot", "Assigned To Quick Slot");
	}
	if (InItemInstance->IsConsumableItem())
	{
		return LOCTEXT("ItemStateUsable", "Usable");
	}

	return LOCTEXT("ItemStateStored", "Stored");
}

#undef LOCTEXT_NAMESPACE

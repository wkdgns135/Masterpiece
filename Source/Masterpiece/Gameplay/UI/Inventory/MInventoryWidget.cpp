#include "Gameplay/UI/Inventory/MInventoryWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Gameplay/Character/Player/Component/MPlayerInventoryComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Item/MItemInstance.h"
#include "Gameplay/UI/Inventory/MInventoryEquipmentSlotWidget.h"
#include "Gameplay/UI/Inventory/MInventoryItemSlotWidget.h"
#include "Gameplay/UI/MUIGameplayTags.h"

UMInventoryWidget::UMInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPolicy = EMUIInputPolicy::Menu;
	WidgetTag = MUIGameplayTags::UI_Widget_GameMenu_Inventory;
}

void UMInventoryWidget::InitializeForPlayerCharacter(AMPlayerCharacterBase* InPlayerCharacter)
{
	UnbindInventoryComponentEvents();
	BoundPlayerCharacter = InPlayerCharacter;
	BoundInventoryComponent = InPlayerCharacter ? InPlayerCharacter->GetInventoryComponent() : nullptr;
	BindInventoryComponentEvents();
	RefreshInventoryView();
}

void UMInventoryWidget::RefreshInventoryView()
{
	TArray<UMItemInstance*> InventoryItems;
	if (BoundInventoryComponent.IsValid())
	{
		BoundInventoryComponent->GetInventoryItems(InventoryItems);
	}

	SynchronizeInventoryItemWidgets(InventoryItems);
	SynchronizeEquipmentSlotWidgets();
	InvalidateLayoutAndVolatility();
}

void UMInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BoundPlayerCharacter)
	{
		BoundPlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwningPlayerPawn());
	}

	if (!BoundInventoryComponent.IsValid())
	{
		BoundInventoryComponent = BoundPlayerCharacter ? BoundPlayerCharacter->GetInventoryComponent() : nullptr;
	}

	BindInventoryComponentEvents();
	RefreshInventoryView();
}

void UMInventoryWidget::NativeDestruct()
{
	UnbindInventoryComponentEvents();
	BoundInventoryComponent = nullptr;
	BoundPlayerCharacter = nullptr;
	ClearInventoryItemWidgets();
	ClearEquipmentSlotWidgets();

	Super::NativeDestruct();
}

void UMInventoryWidget::BindInventoryComponentEvents()
{
	if (!BoundInventoryComponent.IsValid())
	{
		return;
	}

	BoundInventoryComponent->OnInventoryStateChanged.RemoveAll(this);
	BoundInventoryComponent->OnInventoryStateChanged.AddUObject(this, &ThisClass::HandleInventoryStateChanged);
}

void UMInventoryWidget::UnbindInventoryComponentEvents()
{
	if (!BoundInventoryComponent.IsValid())
	{
		return;
	}

	BoundInventoryComponent->OnInventoryStateChanged.RemoveAll(this);
}

void UMInventoryWidget::HandleInventoryStateChanged()
{
	RefreshInventoryView();
}

void UMInventoryWidget::SynchronizeInventoryItemWidgets(const TArray<UMItemInstance*>& InInventoryItems)
{
	ClearInventoryItemWidgets();

	if (!InventoryItemContainer || !InventoryItemSlotWidgetClass)
	{
		return;
	}

	for (int32 ItemIndex = 0; ItemIndex < InInventoryItems.Num(); ++ItemIndex)
	{
		UMItemInstance* ItemInstance = InInventoryItems[ItemIndex];
		if (!ItemInstance)
		{
			continue;
		}

		UMInventoryItemSlotWidget* ItemSlotWidget = CreateInventoryItemSlotWidget();
		if (!ItemSlotWidget)
		{
			continue;
		}

		AddInventoryItemWidgetToContainer(ItemSlotWidget, ItemIndex);
		ItemSlotWidget->InitializeWidget(ItemInstance);
		InventoryItemWidgets.Add(ItemSlotWidget);
	}
}

void UMInventoryWidget::SynchronizeEquipmentSlotWidgets()
{
	ClearEquipmentSlotWidgets();

	if (!EquipmentSlotContainer || !EquipmentSlotWidgetClass)
	{
		return;
	}

	TArray<FGameplayTag> DisplaySlotTags = EquipmentSlotTags;
	if (DisplaySlotTags.Num() <= 0 && BoundInventoryComponent.IsValid())
	{
		TArray<UMItemInstance*> EquippedItems;
		BoundInventoryComponent->GetEquippedItems(EquippedItems);

		for (UMItemInstance* EquippedItem : EquippedItems)
		{
			if (EquippedItem && EquippedItem->GetEquippedSlotTag().IsValid())
			{
				DisplaySlotTags.AddUnique(EquippedItem->GetEquippedSlotTag());
			}
		}
	}

	for (int32 SlotIndex = 0; SlotIndex < DisplaySlotTags.Num(); ++SlotIndex)
	{
		const FGameplayTag EquipSlotTag = DisplaySlotTags[SlotIndex];
		if (!EquipSlotTag.IsValid())
		{
			continue;
		}

		UMInventoryEquipmentSlotWidget* EquipmentSlotWidget = CreateEquipmentSlotWidget();
		if (!EquipmentSlotWidget)
		{
			continue;
		}

		AddEquipmentSlotWidgetToContainer(EquipmentSlotWidget, SlotIndex);
		EquipmentSlotWidget->InitializeEquipmentSlot(BoundInventoryComponent.Get(), EquipSlotTag);
		EquipmentSlotWidget->InitializeWidget(BoundInventoryComponent.IsValid() ? BoundInventoryComponent->GetEquippedItemBySlot(EquipSlotTag) : nullptr);
		EquipmentSlotWidgets.Add(EquipmentSlotWidget);
	}
}

void UMInventoryWidget::ClearInventoryItemWidgets()
{
	for (const TObjectPtr<UMInventoryItemSlotWidget>& ItemSlotWidget : InventoryItemWidgets)
	{
		if (IsValid(ItemSlotWidget))
		{
			ItemSlotWidget->RemoveFromParent();
		}
	}

	InventoryItemWidgets.Reset();
}

void UMInventoryWidget::ClearEquipmentSlotWidgets()
{
	for (const TObjectPtr<UMInventoryEquipmentSlotWidget>& EquipmentSlotWidget : EquipmentSlotWidgets)
	{
		if (IsValid(EquipmentSlotWidget))
		{
			EquipmentSlotWidget->RemoveFromParent();
		}
	}

	EquipmentSlotWidgets.Reset();
}

UMInventoryItemSlotWidget* UMInventoryWidget::CreateInventoryItemSlotWidget() const
{
	if (!InventoryItemSlotWidgetClass)
	{
		return nullptr;
	}

	if (WidgetTree)
	{
		if (UMInventoryItemSlotWidget* ItemSlotWidget = WidgetTree->ConstructWidget<UMInventoryItemSlotWidget>(InventoryItemSlotWidgetClass))
		{
			return ItemSlotWidget;
		}
	}

	if (APlayerController* OwningPlayer = GetOwningPlayer())
	{
		return CreateWidget<UMInventoryItemSlotWidget>(OwningPlayer, InventoryItemSlotWidgetClass);
	}

	if (UWorld* World = GetWorld())
	{
		return CreateWidget<UMInventoryItemSlotWidget>(World, InventoryItemSlotWidgetClass);
	}

	return nullptr;
}

UMInventoryEquipmentSlotWidget* UMInventoryWidget::CreateEquipmentSlotWidget() const
{
	if (!EquipmentSlotWidgetClass)
	{
		return nullptr;
	}

	if (WidgetTree)
	{
		if (UMInventoryEquipmentSlotWidget* EquipmentSlotWidget = WidgetTree->ConstructWidget<UMInventoryEquipmentSlotWidget>(EquipmentSlotWidgetClass))
		{
			return EquipmentSlotWidget;
		}
	}

	if (APlayerController* OwningPlayer = GetOwningPlayer())
	{
		return CreateWidget<UMInventoryEquipmentSlotWidget>(OwningPlayer, EquipmentSlotWidgetClass);
	}

	if (UWorld* World = GetWorld())
	{
		return CreateWidget<UMInventoryEquipmentSlotWidget>(World, EquipmentSlotWidgetClass);
	}

	return nullptr;
}

void UMInventoryWidget::AddInventoryItemWidgetToContainer(UMInventoryItemSlotWidget* ItemSlotWidget, const int32 Index) const
{
	if (!InventoryItemContainer || !ItemSlotWidget)
	{
		return;
	}

	InventoryItemContainer->AddChild(ItemSlotWidget);

	if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ItemSlotWidget->Slot))
	{
		const int32 ColumnCount = FMath::Max(1, InventoryColumns);
		GridSlot->SetRow(Index / ColumnCount);
		GridSlot->SetColumn(Index % ColumnCount);
		GridSlot->SetHorizontalAlignment(HAlign_Fill);
		GridSlot->SetVerticalAlignment(VAlign_Fill);
	}
}

void UMInventoryWidget::AddEquipmentSlotWidgetToContainer(UMInventoryEquipmentSlotWidget* EquipmentSlotWidget, const int32 Index) const
{
	if (!EquipmentSlotContainer || !EquipmentSlotWidget)
	{
		return;
	}

	EquipmentSlotContainer->AddChild(EquipmentSlotWidget);

	if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(EquipmentSlotWidget->Slot))
	{
		const int32 ColumnCount = FMath::Max(1, EquipmentColumns);
		GridSlot->SetRow(Index / ColumnCount);
		GridSlot->SetColumn(Index % ColumnCount);
		GridSlot->SetHorizontalAlignment(HAlign_Fill);
		GridSlot->SetVerticalAlignment(VAlign_Fill);
	}
}

#include "Gameplay/UI/Slot/MSlotWidgetBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/Widget.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/Definition/MDefinitionInstance.h"
#include "Gameplay/UI/DragDrop/MDragDropOperation.h"
#include "Gameplay/UI/Tooltip/MDefinitionTooltipWidget.h"
#include "InputCoreTypes.h"

void UMSlotWidgetBase::RefreshWidget()
{
	RefreshDefinitionToolTipWidget();
	NativeRefreshWidget();
	K2_RefreshWidget();
}

void UMSlotWidgetBase::InitializeWidget(UMDefinitionInstance* InDefinitionInstance)
{
	ApplyDefinitionInstance(InDefinitionInstance);
	NativeInitializeWidget();
	K2_InitializeWidget();
	RefreshWidget();
}

void UMSlotWidgetBase::ClearWidget()
{
	ApplyDefinitionInstance(nullptr);
	NativeClearWidget();
	K2_ClearWidget();
	RefreshWidget();
}

UMDefinitionInstance* UMSlotWidgetBase::GetDefinitionInstance() const
{
	return CachedDefinitionInstance.Get();
}

FGameplayTag UMSlotWidgetBase::GetDefinitionTag() const
{
	return CachedDefinitionInstance ? CachedDefinitionInstance->GetDefinitionTag() : FGameplayTag();
}

void UMSlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshWidget();
}

void UMSlotWidgetBase::NativeDestruct()
{
	ApplyDefinitionInstance(nullptr);
	ClearToolTipWidget(true);

	Super::NativeDestruct();
}

FReply UMSlotWidgetBase::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CanDragDefinitionInstance())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UMSlotWidgetBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!HandleDragDetectedInternal(OutOperation))
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UMSlotWidgetBase::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UMDefinitionInstance* DefinitionInstance = ResolveDefinitionInstance(InOperation))
	{
		if (CanDropDefinitionInstance(DefinitionInstance))
		{
			return true;
		}
	}

	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UMSlotWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UMDefinitionInstance* DefinitionInstance = ResolveDefinitionInstance(InOperation))
	{
		if (CanDropDefinitionInstance(DefinitionInstance) && DropDefinitionInstance(DefinitionInstance))
		{
			return true;
		}
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UMSlotWidgetBase::NativeInitializeWidget()
{
}

void UMSlotWidgetBase::NativeRefreshWidget()
{
}

void UMSlotWidgetBase::NativeClearWidget()
{
}

UImage* UMSlotWidgetBase::GetDragHandleImage_Implementation() const
{
	return nullptr;
}

UMDefinitionInstance* UMSlotWidgetBase::GetDraggableDefinitionInstance_Implementation() const
{
	return GetDefinitionInstance();
}

bool UMSlotWidgetBase::CanDragDefinitionInstance_Implementation() const
{
	return false;
}

bool UMSlotWidgetBase::CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const
{
	(void)DefinitionInstance;
	return false;
}

bool UMSlotWidgetBase::DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance)
{
	(void)DefinitionInstance;
	return false;
}

UMDefinitionInstance* UMSlotWidgetBase::GetToolTipDefinitionInstance_Implementation() const
{
	return GetDefinitionInstance();
}

void UMSlotWidgetBase::RefreshDefinitionToolTipWidget()
{
	if (!bEnableDefinitionToolTip)
	{
		ClearToolTipWidget();
		return;
	}

	UMDefinitionInstance* DefinitionInstance = GetToolTipDefinitionInstance();
	if (!DefinitionInstance)
	{
		ClearToolTipWidget();
		return;
	}

	UMDefinitionTooltipWidget* DefinitionToolTipWidget = GetOrCreateToolTipWidget();
	if (!DefinitionToolTipWidget)
	{
		ClearToolTipWidget();
		return;
	}

	DefinitionToolTipWidget->SetDefinitionInstance(DefinitionInstance);
	SetToolTip(DefinitionToolTipWidget);
}

bool UMSlotWidgetBase::HandleDragDetectedInternal(UDragDropOperation*& OutOperation) const
{
	OutOperation = nullptr;

	if (!CanDragDefinitionInstance())
	{
		return false;
	}

	UMDefinitionInstance* DefinitionInstance = GetDraggableDefinitionInstance();
	if (!DefinitionInstance)
	{
		return false;
	}

	UMDragDropOperation* DragDropOperation = Cast<UMDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(UMDragDropOperation::StaticClass()));
	if (!DragDropOperation)
	{
		return false;
	}

	DragDropOperation->DefinitionInstance = DefinitionInstance;
	DragDropOperation->Payload = DefinitionInstance;
	DragDropOperation->SourceWidget = const_cast<UMSlotWidgetBase*>(this);
	DragDropOperation->Tag = DefinitionInstance->GetDefinitionTag().IsValid() ? DefinitionInstance->GetDefinitionTag().ToString() : FString();
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->Offset = FVector2D::ZeroVector;
	DragDropOperation->DefaultDragVisual = CreateDefaultDragVisual();

	OutOperation = DragDropOperation;
	return true;
}

UMDefinitionInstance* UMSlotWidgetBase::ResolveDefinitionInstance(UDragDropOperation* InOperation) const
{
	if (const UMDragDropOperation* DefinitionOperation = Cast<UMDragDropOperation>(InOperation))
	{
		return DefinitionOperation->GetDefinitionInstance();
	}

	return InOperation ? Cast<UMDefinitionInstance>(InOperation->Payload) : nullptr;
}

UWidget* UMSlotWidgetBase::CreateDefaultDragVisual() const
{
	const UImage* DragHandleImage = GetDragHandleImage();
	if (!DragHandleImage)
	{
		return nullptr;
	}

	UImage* DragVisual = NewObject<UImage>(const_cast<UMSlotWidgetBase*>(this));
	if (!DragVisual)
	{
		return nullptr;
	}

	DragVisual->SetBrush(DragHandleImage->GetBrush());
	DragVisual->SetColorAndOpacity(DragHandleImage->GetColorAndOpacity());
	DragVisual->SetRenderTransform(FWidgetTransform());
	DragVisual->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	return DragVisual;
}

UMDefinitionTooltipWidget* UMSlotWidgetBase::GetOrCreateToolTipWidget()
{
	if (!bEnableDefinitionToolTip || !DefinitionToolTipWidgetClass)
	{
		return nullptr;
	}

	if (DefinitionToolTipWidgetInstance && DefinitionToolTipWidgetInstance->GetClass() != DefinitionToolTipWidgetClass)
	{
		ClearToolTipWidget(true);
	}

	if (DefinitionToolTipWidgetInstance)
	{
		return DefinitionToolTipWidgetInstance;
	}

	if (APlayerController* OwningPlayer = GetOwningPlayer())
	{
		DefinitionToolTipWidgetInstance = CreateWidget<UMDefinitionTooltipWidget>(OwningPlayer, DefinitionToolTipWidgetClass);
	}
	else if (UWorld* World = GetWorld())
	{
		DefinitionToolTipWidgetInstance = CreateWidget<UMDefinitionTooltipWidget>(World, DefinitionToolTipWidgetClass);
	}

	return DefinitionToolTipWidgetInstance;
}

void UMSlotWidgetBase::ClearToolTipWidget(const bool bReleaseInstance)
{
	SetToolTip(nullptr);

	if (bReleaseInstance)
	{
		DefinitionToolTipWidgetInstance = nullptr;
	}
}

void UMSlotWidgetBase::ApplyDefinitionInstance(UMDefinitionInstance* InDefinitionInstance)
{
	if (CachedDefinitionInstance)
	{
		CachedDefinitionInstance->OnDefinitionInstanceChanged.RemoveAll(this);
	}

	CachedDefinitionInstance = InDefinitionInstance;

	if (CachedDefinitionInstance)
	{
		CachedDefinitionInstance->OnDefinitionInstanceChanged.AddUObject(this, &ThisClass::HandleDefinitionInstanceChanged);
	}
}

void UMSlotWidgetBase::HandleDefinitionInstanceChanged()
{
	RefreshWidget();
}

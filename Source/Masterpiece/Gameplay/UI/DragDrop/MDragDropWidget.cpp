// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/DragDrop/MDragDropWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/Widget.h"
#include "Gameplay/Definition/MDefinitionInstance.h"
#include "Gameplay/UI/DragDrop/MDragDropOperation.h"
#include "InputCoreTypes.h"
#include "Layout/Visibility.h"

void UMDragDropWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMDragDropWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply UMDragDropWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CanDragDefinitionInstance())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UMDragDropWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!HandleDragDetectedInternal(OutOperation))
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UMDragDropWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

bool UMDragDropWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

UImage* UMDragDropWidget::GetDragHandleImage_Implementation() const
{
	return nullptr;
}

UMDefinitionInstance* UMDragDropWidget::GetDraggableDefinitionInstance_Implementation() const
{
	return nullptr;
}

bool UMDragDropWidget::CanDragDefinitionInstance_Implementation() const
{
	return false;
}

bool UMDragDropWidget::CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const
{
	return false;
}

bool UMDragDropWidget::DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance)
{
	return false;
}

bool UMDragDropWidget::HandleDragDetectedInternal(UDragDropOperation*& OutOperation) const
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
	DragDropOperation->SourceWidget = const_cast<UMDragDropWidget*>(this);
	DragDropOperation->Tag = DefinitionInstance->GetDefinitionTag().IsValid() ? DefinitionInstance->GetDefinitionTag().ToString() : FString();
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->Offset = FVector2D::ZeroVector;
	DragDropOperation->DefaultDragVisual = CreateDefaultDragVisual();

	OutOperation = DragDropOperation;
	return true;
}

UMDefinitionInstance* UMDragDropWidget::ResolveDefinitionInstance(UDragDropOperation* InOperation) const
{
	if (const UMDragDropOperation* DefinitionOperation = Cast<UMDragDropOperation>(InOperation))
	{
		return DefinitionOperation->GetDefinitionInstance();
	}

	return InOperation ? Cast<UMDefinitionInstance>(InOperation->Payload) : nullptr;
}

UWidget* UMDragDropWidget::CreateDefaultDragVisual() const
{
	const UImage* DragHandleImage = GetDragHandleImage();
	if (!DragHandleImage)
	{
		return nullptr;
	}

	UImage* DragVisual = NewObject<UImage>(const_cast<UMDragDropWidget*>(this));
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

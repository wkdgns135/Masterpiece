// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MDragDropWidget.generated.h"

class UDragDropOperation;
class UImage;
class UMDefinitionInstance;
class UWidget;
class FDragDropEvent;
class FReply;
struct FGeometry;
struct FPointerEvent;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMDragDropWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI|DragDrop")
	UImage* GetDragHandleImage() const;
	virtual UImage* GetDragHandleImage_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI|DragDrop")
	UMDefinitionInstance* GetDraggableDefinitionInstance() const;
	virtual UMDefinitionInstance* GetDraggableDefinitionInstance_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI|DragDrop")
	bool CanDragDefinitionInstance() const;
	virtual bool CanDragDefinitionInstance_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI|DragDrop")
	bool CanDropDefinitionInstance(UMDefinitionInstance* DefinitionInstance) const;
	virtual bool CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="UI|DragDrop")
	bool DropDefinitionInstance(UMDefinitionInstance* DefinitionInstance);
	virtual bool DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance);

private:
	bool HandleDragDetectedInternal(UDragDropOperation*& OutOperation) const;
	UMDefinitionInstance* ResolveDefinitionInstance(UDragDropOperation* InOperation) const;
	UWidget* CreateDefaultDragVisual() const;
};

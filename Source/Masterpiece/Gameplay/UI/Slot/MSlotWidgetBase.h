#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "MSlotWidgetBase.generated.h"

class UDragDropOperation;
class UMDefinitionInstance;
class UMDefinitionTooltipWidget;
class UImage;
class UWidget;
class FDragDropEvent;
class FReply;
struct FGeometry;
struct FPointerEvent;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI|Slot")
	void RefreshWidget();

	UFUNCTION(BlueprintImplementableEvent, Category="UI|Slot", meta=(BlueprintProtected="true"))
	void K2_RefreshWidget();

	UFUNCTION(BlueprintCallable, Category="UI|Slot")
	void InitializeWidget(UMDefinitionInstance* InDefinitionInstance);

	UFUNCTION(BlueprintImplementableEvent, Category="UI|Slot", meta=(BlueprintProtected="true"))
	void K2_InitializeWidget();

	UFUNCTION(BlueprintCallable, Category="UI|Slot")
	void ClearWidget();

	UFUNCTION(BlueprintImplementableEvent, Category="UI|Slot", meta=(BlueprintProtected="true"))
	void K2_ClearWidget();

	UFUNCTION(BlueprintPure, Category="UI|Slot")
	UMDefinitionInstance* GetDefinitionInstance() const;

	UFUNCTION(BlueprintPure, Category="UI|Slot")
	FGameplayTag GetDefinitionTag() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeInitializeWidget();
	virtual void NativeRefreshWidget();
	virtual void NativeClearWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="UI|DragDrop", meta=(BlueprintProtected="true"))
	UImage* GetDragHandleImage() const;
	virtual UImage* GetDragHandleImage_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="UI|DragDrop", meta=(BlueprintProtected="true"))
	UMDefinitionInstance* GetDraggableDefinitionInstance() const;
	virtual UMDefinitionInstance* GetDraggableDefinitionInstance_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="UI|DragDrop", meta=(BlueprintProtected="true"))
	bool CanDragDefinitionInstance() const;
	virtual bool CanDragDefinitionInstance_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="UI|DragDrop", meta=(BlueprintProtected="true"))
	bool CanDropDefinitionInstance(UMDefinitionInstance* DefinitionInstance) const;
	virtual bool CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const;

	UFUNCTION(BlueprintNativeEvent, Category="UI|DragDrop", meta=(BlueprintProtected="true"))
	bool DropDefinitionInstance(UMDefinitionInstance* DefinitionInstance);
	virtual bool DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="UI|ToolTip", meta=(BlueprintProtected="true"))
	UMDefinitionInstance* GetToolTipDefinitionInstance() const;
	virtual UMDefinitionInstance* GetToolTipDefinitionInstance_Implementation() const;

	void RefreshDefinitionToolTipWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI|ToolTip")
	bool bEnableDefinitionToolTip = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI|ToolTip")
	TSubclassOf<UMDefinitionTooltipWidget> DefinitionToolTipWidgetClass;

private:
	void ApplyDefinitionInstance(UMDefinitionInstance* InDefinitionInstance);
	void HandleDefinitionInstanceChanged();
	bool HandleDragDetectedInternal(UDragDropOperation*& OutOperation) const;
	UMDefinitionInstance* ResolveDefinitionInstance(UDragDropOperation* InOperation) const;
	UWidget* CreateDefaultDragVisual() const;
	UMDefinitionTooltipWidget* GetOrCreateToolTipWidget();
	void ClearToolTipWidget(bool bReleaseInstance = false);

private:
	UPROPERTY(Transient)
	TObjectPtr<UMDefinitionInstance> CachedDefinitionInstance;

	UPROPERTY(Transient)
	TObjectPtr<UMDefinitionTooltipWidget> DefinitionToolTipWidgetInstance;
};

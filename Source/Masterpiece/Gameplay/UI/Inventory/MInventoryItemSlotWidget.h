#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/Slot/MSlotWidgetBase.h"
#include "MInventoryItemSlotWidget.generated.h"

class UImage;
class UMItemInstance;
class UTextBlock;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMInventoryItemSlotWidget : public UMSlotWidgetBase
{
	GENERATED_BODY()

public:
	UMInventoryItemSlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category="Inventory")
	UMItemInstance* GetItemInstance() const;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeRefreshWidget() override;
	virtual UImage* GetDragHandleImage_Implementation() const override;
	virtual bool CanDragDefinitionInstance_Implementation() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Inventory", meta=(BindWidgetOptional))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, Category="Inventory", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(BlueprintReadOnly, Category="Inventory", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemName;
};

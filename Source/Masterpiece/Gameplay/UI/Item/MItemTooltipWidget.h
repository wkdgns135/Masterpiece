#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/Tooltip/MDefinitionTooltipWidget.h"
#include "MItemTooltipWidget.generated.h"

class UBorder;
class UImage;
class UMItemInstance;
class USizeBox;
class UTextBlock;
class UVerticalBox;

UCLASS(Blueprintable)
class MASTERPIECE_API UMItemTooltipWidget : public UMDefinitionTooltipWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI|Tooltip|Item")
	UMItemInstance* GetItemInstance() const;

protected:
	virtual void NativeConstruct() override;
	virtual void HandleDefinitionInstanceSet() override;

	UFUNCTION(BlueprintImplementableEvent, Category="UI|Tooltip|Item")
	void K2_OnItemInstanceSet(UMItemInstance* InItemInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemType;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemValue;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Item", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemState;

private:
	void EnsureDefaultLayout();
	void RefreshItemTooltip();
	FText BuildTypeText(const UMItemInstance* InItemInstance) const;
	FText BuildQuantityText(const UMItemInstance* InItemInstance) const;
	FText BuildValueText(const UMItemInstance* InItemInstance) const;
	FText BuildStateText(const UMItemInstance* InItemInstance) const;
};

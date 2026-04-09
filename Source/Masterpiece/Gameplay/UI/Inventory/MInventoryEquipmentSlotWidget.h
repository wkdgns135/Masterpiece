#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/UI/Inventory/MInventoryItemSlotWidget.h"
#include "MInventoryEquipmentSlotWidget.generated.h"

class UMDefinitionInstance;
class UMPlayerInventoryComponent;
class UTextBlock;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMInventoryEquipmentSlotWidget : public UMInventoryItemSlotWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Inventory|EquipmentSlot")
	void InitializeEquipmentSlot(UMPlayerInventoryComponent* InInventoryComponent, FGameplayTag InEquipSlotTag);

	UFUNCTION(BlueprintPure, Category="Inventory|EquipmentSlot")
	UMPlayerInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintPure, Category="Inventory|EquipmentSlot")
	FGameplayTag GetEquipSlotTag() const;

protected:
	virtual void NativeRefreshWidget() override;
	virtual bool CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const override;
	virtual bool DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory|EquipmentSlot")
	void K2_OnEquipmentSlotInitialized(UMPlayerInventoryComponent* InInventoryComponent, FGameplayTag InEquipSlotTag);

protected:
	UPROPERTY(BlueprintReadOnly, Category="Inventory|EquipmentSlot", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> EquipSlotName;

private:
	FText BuildEquipSlotNameText() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UMPlayerInventoryComponent> BoundInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|EquipmentSlot", meta=(AllowPrivateAccess="true"))
	FGameplayTag EquipSlotTag;
};

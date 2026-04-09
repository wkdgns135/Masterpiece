#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/Widget/MActivatableWidget.h"
#include "MInventoryWidget.generated.h"

class AMPlayerCharacterBase;
class UPanelWidget;
class UMInventoryEquipmentSlotWidget;
class UMInventoryItemSlotWidget;
class UMItemInstance;
class UMPlayerInventoryComponent;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMInventoryWidget : public UMActivatableWidget
{
	GENERATED_BODY()

public:
	UMInventoryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeForPlayerCharacter(AMPlayerCharacterBase* InPlayerCharacter);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RefreshInventoryView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindInventoryComponentEvents();
	void UnbindInventoryComponentEvents();
	void HandleInventoryStateChanged();
	void SynchronizeInventoryItemWidgets(const TArray<UMItemInstance*>& InInventoryItems);
	void SynchronizeEquipmentSlotWidgets();
	void ClearInventoryItemWidgets();
	void ClearEquipmentSlotWidgets();
	UMInventoryItemSlotWidget* CreateInventoryItemSlotWidget() const;
	UMInventoryEquipmentSlotWidget* CreateEquipmentSlotWidget() const;
	void AddInventoryItemWidgetToContainer(UMInventoryItemSlotWidget* ItemSlotWidget, int32 Index) const;
	void AddEquipmentSlotWidgetToContainer(UMInventoryEquipmentSlotWidget* EquipmentSlotWidget, int32 Index) const;

private:
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UPanelWidget> InventoryItemContainer;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UPanelWidget> EquipmentSlotContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UMInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UMInventoryEquipmentSlotWidget> EquipmentSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Layout", meta=(AllowPrivateAccess="true", ClampMin="1", UIMin="1"))
	int32 InventoryColumns = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Layout", meta=(AllowPrivateAccess="true", ClampMin="1", UIMin="1"))
	int32 EquipmentColumns = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Layout", meta=(AllowPrivateAccess="true"))
	TArray<FGameplayTag> EquipmentSlotTags;

private:
	UPROPERTY(Transient)
	TObjectPtr<AMPlayerCharacterBase> BoundPlayerCharacter;

	TWeakObjectPtr<UMPlayerInventoryComponent> BoundInventoryComponent;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMInventoryItemSlotWidget>> InventoryItemWidgets;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMInventoryEquipmentSlotWidget>> EquipmentSlotWidgets;
};

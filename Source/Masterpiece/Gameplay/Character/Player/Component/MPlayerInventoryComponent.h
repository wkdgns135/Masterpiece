#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Item/MItemInstance.h"
#include "MPlayerInventoryComponent.generated.h"

class UAbilitySystemComponent;
class UMDefinitionSubsystem;
class UMItemDefinition;

DECLARE_MULTICAST_DELEGATE(FOnPlayerInventoryStateChanged);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerEquipmentSlotChanged, const FGameplayTag&, UMItemInstance*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerConsumableSlotChanged, const FGameplayTag&, UMItemInstance*);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMPlayerInventoryComponent();

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool GetInventoryItems(TArray<UMItemInstance*>& OutInventoryItems) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool GetEquippedItems(TArray<UMItemInstance*>& OutEquippedItems) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	UMItemInstance* FindInventoryItemByTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	UMItemInstance* GetEquippedItemBySlot(FGameplayTag EquipSlotTag) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	UMItemInstance* GetAssignedConsumableItem(FGameplayTag QuickSlotTag) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	FGameplayTag GetAssignedConsumableTag(FGameplayTag QuickSlotTag) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemByTag(FGameplayTag ItemTag, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemDefinition(UMItemDefinition* ItemDefinition, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveInventoryItem(UMItemInstance* ItemInstance, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool EquipItem(UMItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool UnequipItem(FGameplayTag EquipSlotTag);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AssignConsumableToSlot(UMItemInstance* ItemInstance, FGameplayTag QuickSlotTag);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool ClearConsumableSlot(FGameplayTag QuickSlotTag);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool ConsumeAssignedItem(FGameplayTag QuickSlotTag);

public:
	FOnPlayerInventoryStateChanged OnInventoryStateChanged;
	FOnPlayerEquipmentSlotChanged OnEquipmentSlotChanged;
	FOnPlayerConsumableSlotChanged OnConsumableSlotChanged;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UMItemDefinition* ResolveItemDefinition(FGameplayTag ItemTag) const;
	UMDefinitionSubsystem* ResolveDefinitionSubsystem() const;
	UAbilitySystemComponent* ResolveAbilitySystemComponent() const;
	UMItemInstance* CreateItemInstance(UMItemDefinition* ItemDefinition, int32 Quantity);
	bool IsInventoryItemInstance(const UMItemInstance* ItemInstance) const;
	bool ClearEquippedSlotInternal(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EquipSlotTag, UMItemInstance** OutRemovedItem = nullptr);
	bool ClearConsumableSlotInternal(FGameplayTag QuickSlotTag, UMItemInstance** OutRemovedItem = nullptr);
	bool ApplyEquipmentEffect(UAbilitySystemComponent* AbilitySystemComponent, UMItemInstance* ItemInstance, FActiveGameplayEffectHandle& OutHandle) const;
	void RemoveEquipmentEffect(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EquipSlotTag);
	void BroadcastInventoryStateChanged();
	void BroadcastEquipmentSlotChanged(FGameplayTag EquipSlotTag);
	void BroadcastConsumableSlotChanged(FGameplayTag QuickSlotTag);
	void ClearConsumableAssignmentsForItem(UMItemInstance* ItemInstance);
	void ClearEquipmentState();

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UMItemInstance>> InventoryItems;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMItemInstance>> EquippedItems;

	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UMItemInstance>> EquippedItemsBySlot;

	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UMItemInstance>> AssignedConsumableItemsBySlot;

	TMap<FGameplayTag, FActiveGameplayEffectHandle> EquippedEffectHandlesBySlot;
};

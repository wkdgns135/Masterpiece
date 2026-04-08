#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Definition/MDefinitionInstance.h"
#include "Gameplay/Item/MItemDefinition.h"
#include "MItemInstance.generated.h"

class UTexture2D;

UCLASS(BlueprintType)
class MASTERPIECE_API UMItemInstance : public UMDefinitionInstance
{
	GENERATED_BODY()

public:
	bool InitializeItemInstance(UMItemDefinition* InDefinition, int32 InQuantity = 1);

	UFUNCTION(BlueprintPure, Category="Item")
	UMItemDefinition* GetItemDefinition() const;

	UFUNCTION(BlueprintPure, Category="Item")
	UMItemDefinitionEquipment* GetEquipmentDefinition() const;

	UFUNCTION(BlueprintPure, Category="Item")
	UMItemDefinitionConsumable* GetConsumableDefinition() const;

	UFUNCTION(BlueprintPure, Category="Item")
	UMItemDefinitionQuest* GetQuestDefinition() const;

	UFUNCTION(BlueprintPure, Category="Item")
	UMItemDefinitionMisc* GetMiscDefinition() const;

	UFUNCTION(BlueprintPure, Category="Item")
	FGameplayTag GetItemTag() const;

	UFUNCTION(BlueprintPure, Category="Item")
	bool MatchesItemTag(FGameplayTag InItemTag) const;

	UFUNCTION(BlueprintPure, Category="Item")
	TSoftObjectPtr<UTexture2D> GetIcon() const;

	UFUNCTION(BlueprintPure, Category="Item")
	FText GetDisplayName() const;

	UFUNCTION(BlueprintPure, Category="Item")
	FText GetDescription() const;

	UFUNCTION(BlueprintPure, Category="Item")
	bool IsEquipmentItem() const;

	UFUNCTION(BlueprintPure, Category="Item")
	bool IsConsumableItem() const;

	UFUNCTION(BlueprintPure, Category="Item")
	bool IsQuestItem() const;

	UFUNCTION(BlueprintPure, Category="Item")
	bool IsMiscItem() const;

	UFUNCTION(BlueprintPure, Category="Item")
	bool IsStackable() const;

	UFUNCTION(BlueprintPure, Category="Item")
	int32 GetMaxStackCount() const;

	UFUNCTION(BlueprintPure, Category="Item")
	int32 GetBuyPrice() const;

	UFUNCTION(BlueprintPure, Category="Item")
	int32 GetSellPrice() const;

	UFUNCTION(BlueprintPure, Category="Item")
	int32 GetQuantity() const;

	void SetQuantity(int32 InQuantity);

public:
	UPROPERTY(BlueprintReadOnly, Category="Item")
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	bool bStackable = false;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	int32 MaxStackCount = 1;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	int32 BuyPrice = 0;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	int32 SellPrice = 0;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	int32 Quantity = 1;

private:
	const UMItemDefinition* GetItemDefinitionChecked() const;
	void RefreshItemViewData();
};

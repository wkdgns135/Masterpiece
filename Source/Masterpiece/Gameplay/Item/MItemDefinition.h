#pragma once

#include "CoreMinimal.h"
#include "Core/Types/MItemTypes.h"
#include "Gameplay/Definition/MDefinitionObject.h"
#include "MItemDefinition.generated.h"

class UTexture2D;
class UGameplayEffect;

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMItemDefinition : public UMDefinitionObject
{
	GENERATED_BODY()

public:
	FGameplayTag GetItemTag() const;
	bool MatchesItemTag(FGameplayTag InItemTag) const;
	const TSoftObjectPtr<UTexture2D>& GetIcon() const;
	const FText& GetDisplayName() const;
	const FText& GetDescription() const;
	int32 GetMaxStackCount() const;
	int32 GetBuyPrice() const;
	int32 GetSellPrice() const;
	bool IsStackable() const;

	virtual bool IsEquipmentDefinition() const;
	virtual bool IsConsumableDefinition() const;
	virtual bool IsQuestDefinition() const;
	virtual bool IsMiscDefinition() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|UI")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|UI", meta=(MultiLine="true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Stack", meta=(ClampMin="1", UIMin="1"))
	int32 MaxStackCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Economy", meta=(ClampMin="0", UIMin="0"))
	int32 BuyPrice = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Economy", meta=(ClampMin="0", UIMin="0"))
	int32 SellPrice = 0;
};

UCLASS(BlueprintType)
class MASTERPIECE_API UMItemDefinitionEquipment : public UMItemDefinition
{
	GENERATED_BODY()

public:
	FGameplayTag GetEquipSlotTag() const;
	const TSoftClassPtr<UGameplayEffect>& GetEquipGameplayEffectClass() const;
	const TArray<FItemStatModifier>& GetStatModifiers() const;

	virtual bool IsEquipmentDefinition() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Equipment")
	FGameplayTag EquipSlotTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Equipment")
	TSoftClassPtr<UGameplayEffect> EquipGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Equipment")
	TArray<FItemStatModifier> StatModifiers;
};

UCLASS(BlueprintType)
class MASTERPIECE_API UMItemDefinitionConsumable : public UMItemDefinition
{
	GENERATED_BODY()

public:
	const TSoftClassPtr<UGameplayEffect>& GetConsumeGameplayEffectClass() const;
	int32 GetConsumeCountPerUse() const;
	float GetCooldownSeconds() const;

	virtual bool IsConsumableDefinition() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Consumable")
	TSoftClassPtr<UGameplayEffect> ConsumeGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Consumable", meta=(ClampMin="1", UIMin="1"))
	int32 ConsumeCountPerUse = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Consumable", meta=(ClampMin="0.0", UIMin="0.0"))
	float CooldownSeconds = 0.0f;
};

UCLASS(BlueprintType)
class MASTERPIECE_API UMItemDefinitionQuest : public UMItemDefinition
{
	GENERATED_BODY()

public:
	FGameplayTag GetQuestTag() const;
	FGameplayTag GetQuestObjectiveTag() const;
	bool ShouldConsumeOnSubmit() const;

	virtual bool IsQuestDefinition() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Quest")
	FGameplayTag QuestTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Quest")
	FGameplayTag QuestObjectiveTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Quest")
	bool bConsumeOnSubmit = false;
};

UCLASS(BlueprintType)
class MASTERPIECE_API UMItemDefinitionMisc : public UMItemDefinition
{
	GENERATED_BODY()

public:
	const FGameplayTagContainer& GetUsageTags() const;
	bool HasUsageTag(FGameplayTag InUsageTag) const;

	virtual bool IsMiscDefinition() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Misc")
	FGameplayTagContainer UsageTags;
};

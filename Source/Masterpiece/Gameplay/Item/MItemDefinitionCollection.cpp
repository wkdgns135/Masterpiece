#include "Gameplay/Item/MItemDefinitionCollection.h"

#include "Gameplay/Definition/MDefinitionObject.h"
#include "Gameplay/Item/MItemDefinition.h"
#include "Gameplay/Item/MItemGameplayTags.h"

const TArray<TObjectPtr<UMItemDefinition>>& UMItemDefinitionCollection::GetItemDefinitions() const
{
	return ItemDefinitions;
}

UMItemDefinition* UMItemDefinitionCollection::FindItemDefinitionByTag(const FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid())
	{
		return nullptr;
	}

	for (UMItemDefinition* ItemDefinition : ItemDefinitions)
	{
		if (!ItemDefinition)
		{
			continue;
		}

		if (ItemDefinition->GetItemTag().MatchesTagExact(ItemTag))
		{
			return ItemDefinition;
		}
	}

	return nullptr;
}

void UMItemDefinitionCollection::GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const
{
	OutDefinitions.Reset();
	OutDefinitions.Reserve(ItemDefinitions.Num());

	for (UMItemDefinition* ItemDefinition : ItemDefinitions)
	{
		if (ItemDefinition)
		{
			OutDefinitions.Add(ItemDefinition);
		}
	}
}

#if WITH_EDITOR
EDataValidationResult UMItemDefinitionCollection::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	bool bHasError = false;
	TMap<FGameplayTag, UMItemDefinition*> ItemDefinitionByTag;

	for (int32 Index = 0; Index < ItemDefinitions.Num(); ++Index)
	{
		UMItemDefinition* ItemDefinition = ItemDefinitions[Index];
		if (!ItemDefinition)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Item definition collection contains an empty item definition at index %d."), Index)));
			bHasError = true;
			continue;
		}

		const FGameplayTag ItemTag = ItemDefinition->GetItemTag();
		if (!ItemTag.IsValid())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Item definition at index %d has an invalid DefinitionTag."), Index)));
			bHasError = true;
			continue;
		}

		if (ItemDefinitionByTag.Contains(ItemTag))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Item definition collection contains a duplicated item tag: %s"), *ItemTag.ToString())));
			bHasError = true;
			continue;
		}

		if (ItemDefinition->GetMaxStackCount() <= 0)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: MaxStackCount must be greater than 0."), *ItemTag.ToString())));
			bHasError = true;
		}

		if (ItemDefinition->GetBuyPrice() < 0 || ItemDefinition->GetSellPrice() < 0)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: BuyPrice and SellPrice must be non-negative."), *ItemTag.ToString())));
			bHasError = true;
		}

		if (const UMItemDefinitionEquipment* EquipmentDefinition = Cast<UMItemDefinitionEquipment>(ItemDefinition))
		{
			if (!MItemGameplayTags::IsEquipmentTag(ItemTag))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: equipment item DefinitionTag must match Item.Equipment.*"), *ItemTag.ToString())));
				bHasError = true;
			}

			if (!EquipmentDefinition->GetEquipSlotTag().IsValid())
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: equipment item requires a valid EquipSlotTag."), *ItemTag.ToString())));
				bHasError = true;
			}
		}

		if (const UMItemDefinitionConsumable* ConsumableDefinition = Cast<UMItemDefinitionConsumable>(ItemDefinition))
		{
			if (!MItemGameplayTags::IsConsumableTag(ItemTag))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: consumable item DefinitionTag must match Item.Consumable.*"), *ItemTag.ToString())));
				bHasError = true;
			}

			if (ConsumableDefinition->GetConsumeCountPerUse() <= 0)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: consumable item requires ConsumeCountPerUse to be greater than 0."), *ItemTag.ToString())));
				bHasError = true;
			}

			if (ConsumableDefinition->GetCooldownSeconds() < 0.0f)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: consumable item requires CooldownSeconds to be non-negative."), *ItemTag.ToString())));
				bHasError = true;
			}
		}

		if (Cast<UMItemDefinitionQuest>(ItemDefinition) && !MItemGameplayTags::IsQuestTag(ItemTag))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: quest item DefinitionTag must match Item.Quest.*"), *ItemTag.ToString())));
			bHasError = true;
		}

		if (Cast<UMItemDefinitionMisc>(ItemDefinition) && !MItemGameplayTags::IsMiscTag(ItemTag))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: misc item DefinitionTag must match Item.Misc.*"), *ItemTag.ToString())));
			bHasError = true;
		}

		ItemDefinitionByTag.Add(ItemTag, ItemDefinition);
	}

	return bHasError ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

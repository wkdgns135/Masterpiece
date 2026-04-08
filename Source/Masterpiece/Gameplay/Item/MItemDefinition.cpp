#include "Gameplay/Item/MItemDefinition.h"

FGameplayTag UMItemDefinition::GetItemTag() const
{
	return GetDefinitionTag();
}

bool UMItemDefinition::MatchesItemTag(const FGameplayTag InItemTag) const
{
	return GetDefinitionTag().IsValid() && InItemTag.IsValid() && GetDefinitionTag().MatchesTag(InItemTag);
}

const TSoftObjectPtr<UTexture2D>& UMItemDefinition::GetIcon() const
{
	return Icon;
}

const FText& UMItemDefinition::GetDisplayName() const
{
	return DisplayName;
}

const FText& UMItemDefinition::GetDescription() const
{
	return Description;
}

int32 UMItemDefinition::GetMaxStackCount() const
{
	return MaxStackCount;
}

int32 UMItemDefinition::GetBuyPrice() const
{
	return BuyPrice;
}

int32 UMItemDefinition::GetSellPrice() const
{
	return SellPrice;
}

bool UMItemDefinition::IsStackable() const
{
	return MaxStackCount > 1;
}

bool UMItemDefinition::IsEquipmentDefinition() const
{
	return false;
}

bool UMItemDefinition::IsConsumableDefinition() const
{
	return false;
}

bool UMItemDefinition::IsQuestDefinition() const
{
	return false;
}

bool UMItemDefinition::IsMiscDefinition() const
{
	return false;
}

FGameplayTag UMItemDefinitionEquipment::GetEquipSlotTag() const
{
	return EquipSlotTag;
}

const TSoftClassPtr<UGameplayEffect>& UMItemDefinitionEquipment::GetEquipGameplayEffectClass() const
{
	return EquipGameplayEffectClass;
}

const TArray<FItemStatModifier>& UMItemDefinitionEquipment::GetStatModifiers() const
{
	return StatModifiers;
}

bool UMItemDefinitionEquipment::IsEquipmentDefinition() const
{
	return true;
}

const TSoftClassPtr<UGameplayEffect>& UMItemDefinitionConsumable::GetConsumeGameplayEffectClass() const
{
	return ConsumeGameplayEffectClass;
}

int32 UMItemDefinitionConsumable::GetConsumeCountPerUse() const
{
	return ConsumeCountPerUse;
}

float UMItemDefinitionConsumable::GetCooldownSeconds() const
{
	return CooldownSeconds;
}

bool UMItemDefinitionConsumable::IsConsumableDefinition() const
{
	return true;
}

FGameplayTag UMItemDefinitionQuest::GetQuestTag() const
{
	return QuestTag;
}

FGameplayTag UMItemDefinitionQuest::GetQuestObjectiveTag() const
{
	return QuestObjectiveTag;
}

bool UMItemDefinitionQuest::ShouldConsumeOnSubmit() const
{
	return bConsumeOnSubmit;
}

bool UMItemDefinitionQuest::IsQuestDefinition() const
{
	return true;
}

const FGameplayTagContainer& UMItemDefinitionMisc::GetUsageTags() const
{
	return UsageTags;
}

bool UMItemDefinitionMisc::HasUsageTag(const FGameplayTag InUsageTag) const
{
	return InUsageTag.IsValid() && UsageTags.HasTagExact(InUsageTag);
}

bool UMItemDefinitionMisc::IsMiscDefinition() const
{
	return true;
}

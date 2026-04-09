#include "Gameplay/Item/MItemGameplayTags.h"

namespace MItemGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Item, "Item");

	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment, "Item.Equipment");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Weapon, "Item.Equipment.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor, "Item.Equipment.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Accessory, "Item.Equipment.Accessory");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Special, "Item.Equipment.Special");

	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable, "Item.Consumable");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_Potion, "Item.Consumable.Potion");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_Food, "Item.Consumable.Food");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_Buff, "Item.Consumable.Buff");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_Scroll, "Item.Consumable.Scroll");

	UE_DEFINE_GAMEPLAY_TAG(Item_Quest, "Item.Quest");
	UE_DEFINE_GAMEPLAY_TAG(Item_Quest_Main, "Item.Quest.Main");
	UE_DEFINE_GAMEPLAY_TAG(Item_Quest_Side, "Item.Quest.Side");
	UE_DEFINE_GAMEPLAY_TAG(Item_Quest_Event, "Item.Quest.Event");

	UE_DEFINE_GAMEPLAY_TAG(Item_Misc, "Item.Misc");
	UE_DEFINE_GAMEPLAY_TAG(Item_Misc_Material, "Item.Misc.Material");
	UE_DEFINE_GAMEPLAY_TAG(Item_Misc_Currency, "Item.Misc.Currency");
	UE_DEFINE_GAMEPLAY_TAG(Item_Misc_Crafting, "Item.Misc.Crafting");
	UE_DEFINE_GAMEPLAY_TAG(Item_Misc_Key, "Item.Misc.Key");

	bool IsItemTag(const FGameplayTag& InItemTag)
	{
		return InItemTag.IsValid() && InItemTag.MatchesTag(Item);
	}

	bool IsEquipmentTag(const FGameplayTag& InItemTag)
	{
		return InItemTag.IsValid() && InItemTag.MatchesTag(Item_Equipment);
	}

	bool IsConsumableTag(const FGameplayTag& InItemTag)
	{
		return InItemTag.IsValid() && InItemTag.MatchesTag(Item_Consumable);
	}

	bool IsQuestTag(const FGameplayTag& InItemTag)
	{
		return InItemTag.IsValid() && InItemTag.MatchesTag(Item_Quest);
	}

	bool IsMiscTag(const FGameplayTag& InItemTag)
	{
		return InItemTag.IsValid() && InItemTag.MatchesTag(Item_Misc);
	}
}

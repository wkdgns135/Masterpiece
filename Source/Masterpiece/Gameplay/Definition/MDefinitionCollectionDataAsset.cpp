#include "Gameplay/Definition/MDefinitionCollectionDataAsset.h"

#include "Gameplay/Definition/MDefinitionObject.h"

FGameplayTag UMDefinitionCollectionDataAsset::GetCollectionTag() const
{
	return CollectionTag;
}

void UMDefinitionCollectionDataAsset::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (CollectionTag.IsValid())
	{
		TagContainer.AddTag(CollectionTag);
	}
}

void UMDefinitionCollectionDataAsset::GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const
{
	OutDefinitions.Reset();
}

bool UMDefinitionCollectionDataAsset::FindDefinitionByTag(const FGameplayTag DefinitionTag, UMDefinitionObject*& OutDefinition) const
{
	OutDefinition = nullptr;
	if (!DefinitionTag.IsValid())
	{
		return false;
	}

	TArray<UMDefinitionObject*> Definitions;
	GetDefinitions(Definitions);

	for (UMDefinitionObject* Definition : Definitions)
	{
		if (!Definition)
		{
			continue;
		}

		if (Definition->GetDefinitionTag().MatchesTagExact(DefinitionTag))
		{
			OutDefinition = Definition;
			return true;
		}
	}

	return false;
}

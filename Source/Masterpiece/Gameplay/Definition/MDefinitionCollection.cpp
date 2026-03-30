#include "Gameplay/Definition/MDefinitionCollection.h"

#include "Gameplay/Definition/MDefinitionObject.h"

FGameplayTag UMDefinitionCollection::GetCollectionTag() const
{
	return CollectionTag;
}

void UMDefinitionCollection::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (CollectionTag.IsValid())
	{
		TagContainer.AddTag(CollectionTag);
	}
}

void UMDefinitionCollection::GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const
{
	OutDefinitions.Reset();
}

bool UMDefinitionCollection::FindDefinitionByTag(const FGameplayTag DefinitionTag, UMDefinitionObject*& OutDefinition) const
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

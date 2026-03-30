#include "Gameplay/Definition/MDefinitionObject.h"

FGameplayTag UMDefinitionObject::GetDefinitionTag() const
{
	return DefinitionTag;
}

void UMDefinitionObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (DefinitionTag.IsValid())
	{
		TagContainer.AddTag(DefinitionTag);
	}
}

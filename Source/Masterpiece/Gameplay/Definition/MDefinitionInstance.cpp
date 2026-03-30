#include "Gameplay/Definition/MDefinitionInstance.h"

UMDefinitionObject* UMDefinitionInstance::GetDefinition() const
{
	return Definition;
}

FGameplayTag UMDefinitionInstance::GetDefinitionTag() const
{
	return Definition ? Definition->GetDefinitionTag() : FGameplayTag();
}

void UMDefinitionInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (Definition)
	{
		Definition->GetOwnedGameplayTags(TagContainer);
	}
}

void UMDefinitionInstance::InitializeDefinition(UMDefinitionObject* InDefinition)
{
	Definition = InDefinition;
}

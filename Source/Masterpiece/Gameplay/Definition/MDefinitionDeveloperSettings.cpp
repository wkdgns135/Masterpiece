#include "Gameplay/Definition/MDefinitionDeveloperSettings.h"

FName UMDefinitionDeveloperSettings::GetCategoryName() const
{
	return TEXT("Game");
}

const TArray<TSoftObjectPtr<UMDefinitionRegistryDataAsset>>& UMDefinitionDeveloperSettings::GetDefinitionRegistries() const
{
	return DefinitionRegistries;
}

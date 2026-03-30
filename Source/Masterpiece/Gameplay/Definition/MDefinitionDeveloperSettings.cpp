#include "Gameplay/Definition/MDefinitionDeveloperSettings.h"

FName UMDefinitionDeveloperSettings::GetCategoryName() const
{
	return TEXT("Game");
}

const TArray<TSoftObjectPtr<UMDefinitionRegistry>>& UMDefinitionDeveloperSettings::GetDefinitionRegistries() const
{
	return DefinitionRegistries;
}

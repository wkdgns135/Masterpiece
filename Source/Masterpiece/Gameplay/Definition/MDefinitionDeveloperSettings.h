#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MDefinitionDeveloperSettings.generated.h"

class UMDefinitionRegistryDataAsset;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Definitions"))
class MASTERPIECE_API UMDefinitionDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override;

	const TArray<TSoftObjectPtr<UMDefinitionRegistryDataAsset>>& GetDefinitionRegistries() const;

private:
	UPROPERTY(Config, EditAnywhere, Category="Definition")
	TArray<TSoftObjectPtr<UMDefinitionRegistryDataAsset>> DefinitionRegistries;
};

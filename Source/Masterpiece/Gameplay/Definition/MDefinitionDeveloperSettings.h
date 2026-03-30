#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MDefinitionDeveloperSettings.generated.h"

class UMDefinitionRegistry;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Definitions"))
class MASTERPIECE_API UMDefinitionDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override;

	const TArray<TSoftObjectPtr<UMDefinitionRegistry>>& GetDefinitionRegistries() const;

private:
	UPROPERTY(Config, EditAnywhere, Category="Definition")
	TArray<TSoftObjectPtr<UMDefinitionRegistry>> DefinitionRegistries;
};

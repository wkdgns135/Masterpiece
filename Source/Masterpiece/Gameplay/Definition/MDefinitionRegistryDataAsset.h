#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "MDefinitionRegistryDataAsset.generated.h"

class UMDefinitionCollectionDataAsset;

UCLASS(BlueprintType)
class MASTERPIECE_API UMDefinitionRegistryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	bool GetCollectionEntries(TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollectionDataAsset>>& OutEntries) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Definition", meta=(AllowPrivateAccess="true"))
	TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollectionDataAsset>> CollectionEntries;
};

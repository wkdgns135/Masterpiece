#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "MDefinitionRegistry.generated.h"

class UMDefinitionCollection;

UCLASS(BlueprintType)
class MASTERPIECE_API UMDefinitionRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	bool GetCollectionEntries(TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>>& OutEntries) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Definition", meta=(AllowPrivateAccess="true"))
	TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>> CollectionEntries;
};

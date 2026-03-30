#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "MDefinitionCollectionDataAsset.generated.h"

class UMDefinitionObject;

UCLASS(Abstract, BlueprintType)
class MASTERPIECE_API UMDefinitionCollectionDataAsset : public UPrimaryDataAsset, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Definition")
	FGameplayTag GetCollectionTag() const;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual void GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const;

	bool FindDefinitionByTag(FGameplayTag DefinitionTag, UMDefinitionObject*& OutDefinition) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ToolTip="서브시스템이 이 정의 컬렉션을 로드할 때 사용하는 고유 게임플레이 태그입니다."))
	FGameplayTag CollectionTag;
};

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "MDefinitionObject.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMDefinitionObject : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Definition")
	FGameplayTag GetDefinitionTag() const;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ToolTip="데이터베이스와 런타임에서 이 정의를 식별할 고유 게임플레이 태그입니다."))
	FGameplayTag DefinitionTag;
};

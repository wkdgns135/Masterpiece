#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Gameplay/Definition/MDefinitionObject.h"
#include "UObject/Object.h"
#include "MDefinitionInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDefinitionInstanceChanged);

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMDefinitionInstance : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Definition")
	UMDefinitionObject* GetDefinition() const;

	UFUNCTION(BlueprintPure, Category="Definition")
	FGameplayTag GetDefinitionTag() const;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

public:
	FOnDefinitionInstanceChanged OnDefinitionInstanceChanged;
	
protected:
	void InitializeDefinition(UMDefinitionObject* InDefinition);
	void BroadcastDefinitionInstanceChanged();

protected:
	UPROPERTY(Transient)
	TObjectPtr<UMDefinitionObject> Definition;
};

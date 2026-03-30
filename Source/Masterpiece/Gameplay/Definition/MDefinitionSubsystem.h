#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MDefinitionSubsystem.generated.h"

struct FStreamableHandle;
class UMDefinitionCollectionDataAsset;
class UMDefinitionObject;

DECLARE_DELEGATE_OneParam(FOnDefinitionCollectionLoaded, UMDefinitionCollectionDataAsset*);

UCLASS()
class MASTERPIECE_API UMDefinitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Definition")
	UMDefinitionCollectionDataAsset* GetCollectionByTag(FGameplayTag CollectionTag) const;

	UFUNCTION(BlueprintPure, Category="Definition")
	UMDefinitionObject* GetDefinitionByTag(FGameplayTag DefinitionTag) const;

	template <typename TCollection>
	TCollection* GetCollection(FGameplayTag CollectionTag) const
	{
		return Cast<TCollection>(GetCollectionByTag(CollectionTag));
	}

	template <typename TDefinition>
	TDefinition* GetDefinition(FGameplayTag DefinitionTag) const
	{
		return Cast<TDefinition>(GetDefinitionByTag(DefinitionTag));
	}

	bool LoadCollectionByTagAsync(FGameplayTag CollectionTag, FOnDefinitionCollectionLoaded Callback);

protected:
	virtual void Deinitialize() override;

private:
	void EnsureRegistryInitialized() const;
	UMDefinitionCollectionDataAsset* LoadCollectionByTagSync(FGameplayTag CollectionTag) const;
	void HandleCollectionAsyncLoadComplete(FGameplayTag CollectionTag);
	void IndexDefinitionsFromCollection(UMDefinitionCollectionDataAsset* LoadedCollection) const;
	void ExecutePendingCollectionCallbacks(FGameplayTag CollectionTag, UMDefinitionCollectionDataAsset* LoadedCollection);

private:
	mutable bool bRegistryInitialized = false;

	UPROPERTY(Transient)
	mutable TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollectionDataAsset>> CollectionAssetsByTag;

	UPROPERTY(Transient)
	mutable TMap<FGameplayTag, TObjectPtr<UMDefinitionCollectionDataAsset>> LoadedCollectionsByTag;

	UPROPERTY(Transient)
	mutable TMap<FGameplayTag, TObjectPtr<UMDefinitionObject>> LoadedDefinitionsByTag;

	mutable TMap<FGameplayTag, TSharedPtr<FStreamableHandle>> LoadingCollectionHandles;
	TMap<FGameplayTag, TArray<FOnDefinitionCollectionLoaded>> PendingCollectionCallbacks;
};

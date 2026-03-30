#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MDefinitionSubsystem.generated.h"

struct FStreamableHandle;
class UMDefinitionCollection;
class UMDefinitionObject;

DECLARE_DELEGATE_OneParam(FOnDefinitionCollectionLoaded, UMDefinitionCollection*);

UCLASS()
class MASTERPIECE_API UMDefinitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Definition")
	UMDefinitionCollection* GetCollectionByTag(FGameplayTag CollectionTag) const;

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
	UMDefinitionCollection* LoadCollectionByTagSync(FGameplayTag CollectionTag) const;
	void HandleCollectionAsyncLoadComplete(FGameplayTag CollectionTag);
	void IndexDefinitionsFromCollection(UMDefinitionCollection* LoadedCollection) const;
	void ExecutePendingCollectionCallbacks(FGameplayTag CollectionTag, UMDefinitionCollection* LoadedCollection);

private:
	mutable bool bRegistryInitialized = false;

	UPROPERTY(Transient)
	mutable TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>> CollectionAssetsByTag;

	UPROPERTY(Transient)
	mutable TMap<FGameplayTag, TObjectPtr<UMDefinitionCollection>> LoadedCollectionsByTag;

	UPROPERTY(Transient)
	mutable TMap<FGameplayTag, TObjectPtr<UMDefinitionObject>> LoadedDefinitionsByTag;

	mutable TMap<FGameplayTag, TSharedPtr<FStreamableHandle>> LoadingCollectionHandles;
	TMap<FGameplayTag, TArray<FOnDefinitionCollectionLoaded>> PendingCollectionCallbacks;
};

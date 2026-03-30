#include "Gameplay/Definition/MDefinitionSubsystem.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Gameplay/Definition/MDefinitionCollection.h"
#include "Gameplay/Definition/MDefinitionDeveloperSettings.h"
#include "Gameplay/Definition/MDefinitionObject.h"
#include "Gameplay/Definition/MDefinitionRegistry.h"

UMDefinitionCollection* UMDefinitionSubsystem::GetCollectionByTag(const FGameplayTag CollectionTag) const
{
	if (!CollectionTag.IsValid())
	{
		return nullptr;
	}

	EnsureRegistryInitialized();
	return LoadCollectionByTagSync(CollectionTag);
}

UMDefinitionObject* UMDefinitionSubsystem::GetDefinitionByTag(const FGameplayTag DefinitionTag) const
{
	if (!DefinitionTag.IsValid())
	{
		return nullptr;
	}

	EnsureRegistryInitialized();

	if (const TObjectPtr<UMDefinitionObject>* LoadedDefinition = LoadedDefinitionsByTag.Find(DefinitionTag))
	{
		return LoadedDefinition->Get();
	}

	for (const TPair<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>>& Pair : CollectionAssetsByTag)
	{
		if (!Pair.Key.IsValid())
		{
			continue;
		}

		LoadCollectionByTagSync(Pair.Key);

		if (const TObjectPtr<UMDefinitionObject>* LoadedDefinition = LoadedDefinitionsByTag.Find(DefinitionTag))
		{
			return LoadedDefinition->Get();
		}
	}

	return nullptr;
}

bool UMDefinitionSubsystem::LoadCollectionByTagAsync(const FGameplayTag CollectionTag, FOnDefinitionCollectionLoaded Callback)
{
	if (!CollectionTag.IsValid())
	{
		Callback.ExecuteIfBound(nullptr);
		return false;
	}

	EnsureRegistryInitialized();

	if (const TObjectPtr<UMDefinitionCollection>* LoadedCollection = LoadedCollectionsByTag.Find(CollectionTag))
	{
		Callback.ExecuteIfBound(LoadedCollection->Get());
		return LoadedCollection->Get() != nullptr;
	}

	const TSoftObjectPtr<UMDefinitionCollection>* CollectionAsset = CollectionAssetsByTag.Find(CollectionTag);
	if (!CollectionAsset || CollectionAsset->IsNull())
	{
		Callback.ExecuteIfBound(nullptr);
		return false;
	}

	if (Callback.IsBound())
	{
		PendingCollectionCallbacks.FindOrAdd(CollectionTag).Add(MoveTemp(Callback));
	}

	if (LoadingCollectionHandles.Contains(CollectionTag))
	{
		return true;
	}

	TWeakObjectPtr<UMDefinitionSubsystem> WeakThis(this);
	TSharedPtr<FStreamableHandle> Handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CollectionAsset->ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([WeakThis, CollectionTag]()
		{
			if (!WeakThis.IsValid())
			{
				return;
			}

			WeakThis->HandleCollectionAsyncLoadComplete(CollectionTag);
		}));

	if (!Handle.IsValid())
	{
		UMDefinitionCollection* LoadedCollection = LoadCollectionByTagSync(CollectionTag);
		ExecutePendingCollectionCallbacks(CollectionTag, LoadedCollection);
		return LoadedCollection != nullptr;
	}

	LoadingCollectionHandles.Add(CollectionTag, Handle);
	return true;
}

void UMDefinitionSubsystem::Deinitialize()
{
	CollectionAssetsByTag.Reset();
	LoadedCollectionsByTag.Reset();
	LoadedDefinitionsByTag.Reset();
	LoadingCollectionHandles.Reset();
	PendingCollectionCallbacks.Reset();
	bRegistryInitialized = false;

	Super::Deinitialize();
}

void UMDefinitionSubsystem::EnsureRegistryInitialized() const
{
	if (bRegistryInitialized)
	{
		return;
	}

	bRegistryInitialized = true;
	CollectionAssetsByTag.Reset();
	LoadedCollectionsByTag.Reset();
	LoadedDefinitionsByTag.Reset();

	const UMDefinitionDeveloperSettings* DefinitionSettings = GetDefault<UMDefinitionDeveloperSettings>();
	if (!DefinitionSettings)
	{
		return;
	}

	for (const TSoftObjectPtr<UMDefinitionRegistry>& RegistryAsset : DefinitionSettings->GetDefinitionRegistries())
	{
		if (RegistryAsset.IsNull())
		{
			continue;
		}

		const UMDefinitionRegistry* LoadedRegistry = RegistryAsset.LoadSynchronous();
		if (!LoadedRegistry)
		{
			continue;
		}

		TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>> RegistryEntries;
		LoadedRegistry->GetCollectionEntries(RegistryEntries);

		for (const TPair<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>>& Entry : RegistryEntries)
		{
			if (!Entry.Key.IsValid() || Entry.Value.IsNull())
			{
				continue;
			}

			if (CollectionAssetsByTag.Contains(Entry.Key))
			{
				UE_LOG(LogTemp, Warning, TEXT("DefinitionSubsystem: duplicate collection tag registration detected for %s."), *Entry.Key.ToString());
				continue;
			}

			CollectionAssetsByTag.Add(Entry.Key, Entry.Value);
		}
	}
}

UMDefinitionCollection* UMDefinitionSubsystem::LoadCollectionByTagSync(const FGameplayTag CollectionTag) const
{
	if (const TObjectPtr<UMDefinitionCollection>* LoadedCollection = LoadedCollectionsByTag.Find(CollectionTag))
	{
		return LoadedCollection->Get();
	}

	const TSoftObjectPtr<UMDefinitionCollection>* CollectionAsset = CollectionAssetsByTag.Find(CollectionTag);
	if (!CollectionAsset || CollectionAsset->IsNull())
	{
		return nullptr;
	}

	UMDefinitionCollection* LoadedCollection = CollectionAsset->LoadSynchronous();
	if (!LoadedCollection)
	{
		return nullptr;
	}

	LoadedCollectionsByTag.Add(CollectionTag, LoadedCollection);
	IndexDefinitionsFromCollection(LoadedCollection);
	return LoadedCollection;
}

void UMDefinitionSubsystem::HandleCollectionAsyncLoadComplete(const FGameplayTag CollectionTag)
{
	LoadingCollectionHandles.Remove(CollectionTag);

	UMDefinitionCollection* LoadedCollection = LoadCollectionByTagSync(CollectionTag);
	ExecutePendingCollectionCallbacks(CollectionTag, LoadedCollection);
}

void UMDefinitionSubsystem::IndexDefinitionsFromCollection(UMDefinitionCollection* LoadedCollection) const
{
	if (!LoadedCollection)
	{
		return;
	}

	TArray<UMDefinitionObject*> Definitions;
	LoadedCollection->GetDefinitions(Definitions);

	for (UMDefinitionObject* Definition : Definitions)
	{
		if (!Definition || !Definition->GetDefinitionTag().IsValid())
		{
			continue;
		}

		if (LoadedDefinitionsByTag.Contains(Definition->GetDefinitionTag()))
		{
			UE_LOG(LogTemp, Warning, TEXT("DefinitionSubsystem: duplicate definition tag detected while indexing %s."),
				*Definition->GetDefinitionTag().ToString());
			continue;
		}

		LoadedDefinitionsByTag.Add(Definition->GetDefinitionTag(), Definition);
	}
}

void UMDefinitionSubsystem::ExecutePendingCollectionCallbacks(const FGameplayTag CollectionTag, UMDefinitionCollection* LoadedCollection)
{
	TArray<FOnDefinitionCollectionLoaded> Callbacks;
	PendingCollectionCallbacks.RemoveAndCopyValue(CollectionTag, Callbacks);

	for (FOnDefinitionCollectionLoaded& Callback : Callbacks)
	{
		Callback.ExecuteIfBound(LoadedCollection);
	}
}

#include "Gameplay/Definition/MDefinitionRegistryDataAsset.h"

#include "Gameplay/Definition/MDefinitionCollectionDataAsset.h"

bool UMDefinitionRegistryDataAsset::GetCollectionEntries(TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollectionDataAsset>>& OutEntries) const
{
	OutEntries = CollectionEntries;
	return OutEntries.Num() > 0;
}

#if WITH_EDITOR
EDataValidationResult UMDefinitionRegistryDataAsset::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	bool bHasError = false;
	for (const TPair<FGameplayTag, TSoftObjectPtr<UMDefinitionCollectionDataAsset>>& Pair : CollectionEntries)
	{
		if (!Pair.Key.IsValid())
		{
			Context.AddError(FText::FromString(TEXT("Definition registry contains an invalid collection tag key.")));
			bHasError = true;
			continue;
		}

		if (Pair.Value.IsNull())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: registry entry is missing its collection asset."), *Pair.Key.ToString())));
			bHasError = true;
			continue;
		}

		const UMDefinitionCollectionDataAsset* CollectionAsset = Pair.Value.LoadSynchronous();
		if (!CollectionAsset)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: registry entry failed to load its collection asset."), *Pair.Key.ToString())));
			bHasError = true;
			continue;
		}

		if (!CollectionAsset->GetCollectionTag().MatchesTagExact(Pair.Key))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: registry key does not match asset collection tag %s."),
				*Pair.Key.ToString(),
				*CollectionAsset->GetCollectionTag().ToString())));
			bHasError = true;
		}
	}

	return bHasError ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

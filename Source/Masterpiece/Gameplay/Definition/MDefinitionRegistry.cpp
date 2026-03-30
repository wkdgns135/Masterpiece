#include "Gameplay/Definition/MDefinitionRegistry.h"

#include "Gameplay/Definition/MDefinitionCollection.h"

bool UMDefinitionRegistry::GetCollectionEntries(TMap<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>>& OutEntries) const
{
	OutEntries = CollectionEntries;
	return OutEntries.Num() > 0;
}

#if WITH_EDITOR
EDataValidationResult UMDefinitionRegistry::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	bool bHasError = false;
	for (const TPair<FGameplayTag, TSoftObjectPtr<UMDefinitionCollection>>& Pair : CollectionEntries)
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

		const UMDefinitionCollection* CollectionAsset = Pair.Value.LoadSynchronous();
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

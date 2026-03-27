// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/MUIWidgetRegistryDataAsset.h"

#include "Gameplay/UI/Widget/MTaggedWidget.h"
#include "Misc/DataValidation.h"

namespace
{
#if WITH_EDITOR
void AddValidationError(FDataValidationContext& Context, const FString& Message)
{
	Context.AddError(FText::FromString(Message));
}
#endif
}

TSubclassOf<UMTaggedWidget> UMUIWidgetRegistryDataAsset::FindWidgetClassByTag(const FGameplayTag WidgetTag) const
{
	const TSoftClassPtr<UMTaggedWidget>* WidgetClass = WidgetEntries.Find(WidgetTag);
	if (!WidgetClass)
	{
		return nullptr;
	}

	return WidgetClass->LoadSynchronous();
}

#if WITH_EDITOR
EDataValidationResult UMUIWidgetRegistryDataAsset::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	bool bHasError = false;

	for (const TPair<FGameplayTag, TSoftClassPtr<UMTaggedWidget>>& Pair : WidgetEntries)
	{
		if (!Pair.Key.IsValid())
		{
			AddValidationError(Context, TEXT("WidgetEntries contains an invalid widget tag key."));
			bHasError = true;
			continue;
		}

		if (Pair.Value.IsNull())
		{
			AddValidationError(Context, FString::Printf(TEXT("%s: WidgetClass is not assigned."), *Pair.Key.ToString()));
			bHasError = true;
			continue;
		}

		const TSubclassOf<UMTaggedWidget> WidgetClass = Pair.Value.LoadSynchronous();
		if (!WidgetClass)
		{
			AddValidationError(Context, FString::Printf(TEXT("%s: WidgetClass could not be loaded."), *Pair.Key.ToString()));
			bHasError = true;
			continue;
		}

		const UMTaggedWidget* WidgetCDO = WidgetClass->GetDefaultObject<UMTaggedWidget>();
		if (!WidgetCDO)
		{
			AddValidationError(Context, FString::Printf(TEXT("%s: WidgetClass default object is invalid."), *Pair.Key.ToString()));
			bHasError = true;
			continue;
		}

		if (WidgetCDO->GetWidgetTag() != Pair.Key)
		{
			AddValidationError(Context, FString::Printf(TEXT("%s: registry tag does not match widget tag (%s)."),
				*Pair.Key.ToString(),
				*WidgetCDO->GetWidgetTag().ToString()));
			bHasError = true;
		}
	}

	return bHasError ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

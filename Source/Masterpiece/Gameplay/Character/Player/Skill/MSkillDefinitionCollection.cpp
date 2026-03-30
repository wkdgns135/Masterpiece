#include "Gameplay/Character/Player/Skill/MSkillDefinitionCollection.h"

#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Definition/MDefinitionObject.h"

namespace
{
bool IsValidRequiredCharacterLevel(const int32 Level)
{
	return Level == 1 || (Level > 0 && Level % 5 == 0);
}
}

const TArray<TObjectPtr<UMSkillDefinition>>& UMSkillDefinitionCollection::GetSkillDefinitions() const
{
	return SkillDefinitions;
}

UMSkillDefinition* UMSkillDefinitionCollection::FindSkillDefinitionByTag(const FGameplayTag SkillTag) const
{
	if (!SkillTag.IsValid())
	{
		return nullptr;
	}

	for (UMSkillDefinition* SkillDefinition : SkillDefinitions)
	{
		if (!SkillDefinition)
		{
			continue;
		}

		if (SkillDefinition->GetSkillTag().MatchesTagExact(SkillTag))
		{
			return SkillDefinition;
		}
	}

	return nullptr;
}

void UMSkillDefinitionCollection::GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const
{
	OutDefinitions.Reset();
	OutDefinitions.Reserve(SkillDefinitions.Num());

	for (UMSkillDefinition* SkillDefinition : SkillDefinitions)
	{
		if (SkillDefinition)
		{
			OutDefinitions.Add(SkillDefinition);
		}
	}
}

#if WITH_EDITOR
EDataValidationResult UMSkillDefinitionCollection::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	bool bHasError = false;
	TMap<FGameplayTag, UMSkillDefinition*> SkillDefinitionByTag;
	TMap<FGameplayTag, int32> SkillLevelByTag;
	TMap<FGameplayTag, int32> InDegreeByTag;
	TMap<FGameplayTag, TArray<FGameplayTag>> ChildrenByParentTag;

	for (int32 Index = 0; Index < SkillDefinitions.Num(); ++Index)
	{
		UMSkillDefinition* SkillDefinition = SkillDefinitions[Index];
		if (!SkillDefinition)
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Skill definition collection contains an empty skill definition at index %d."), Index)));
			bHasError = true;
			continue;
		}

		const FGameplayTag SkillTag = SkillDefinition->GetSkillTag();
		if (!SkillTag.IsValid())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Skill definition at index %d has an invalid SkillTag."), Index)));
			bHasError = true;
			continue;
		}

		if (SkillDefinitionByTag.Contains(SkillTag))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Skill definition collection contains a duplicated skill tag: %s"), *SkillTag.ToString())));
			bHasError = true;
			continue;
		}

		if (!IsValidRequiredCharacterLevel(SkillDefinition->GetRequiredCharacterLevel()))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("%s: RequiredCharacterLevel must be 1 or a multiple of 5. Current value: %d"),
				*SkillTag.ToString(),
				SkillDefinition->GetRequiredCharacterLevel())));
			bHasError = true;
		}

		SkillDefinitionByTag.Add(SkillTag, SkillDefinition);
		SkillLevelByTag.Add(SkillTag, SkillDefinition->GetRequiredCharacterLevel());
		InDegreeByTag.Add(SkillTag, 0);
	}

	for (const TPair<FGameplayTag, UMSkillDefinition*>& Pair : SkillDefinitionByTag)
	{
		const FGameplayTag SkillTag = Pair.Key;
		const UMSkillDefinition* SkillDefinition = Pair.Value;
		if (!SkillDefinition)
		{
			continue;
		}

		for (const TPair<FGameplayTag, int32>& PrerequisitePair : SkillDefinition->GetPrerequisiteSkillRanks())
		{
			if (!PrerequisitePair.Key.IsValid())
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: prerequisite tag is invalid."), *SkillTag.ToString())));
				bHasError = true;
				continue;
			}

			if (!SkillDefinitionByTag.Contains(PrerequisitePair.Key))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: references a missing prerequisite skill %s."),
					*SkillTag.ToString(),
					*PrerequisitePair.Key.ToString())));
				bHasError = true;
				continue;
			}

			const int32 ParentLevel = SkillLevelByTag.FindRef(PrerequisitePair.Key);
			const int32 ChildLevel = SkillLevelByTag.FindRef(SkillTag);
			if (!(ParentLevel < ChildLevel))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("%s: prerequisite %s must have a lower RequiredCharacterLevel."),
					*SkillTag.ToString(),
					*PrerequisitePair.Key.ToString())));
				bHasError = true;
			}

			InDegreeByTag.FindOrAdd(SkillTag) += 1;
			ChildrenByParentTag.FindOrAdd(PrerequisitePair.Key).Add(SkillTag);
		}
	}

	TArray<FGameplayTag> ZeroInDegreeQueue;
	for (const TPair<FGameplayTag, int32>& Pair : InDegreeByTag)
	{
		if (Pair.Value == 0)
		{
			ZeroInDegreeQueue.Add(Pair.Key);
		}
	}

	int32 ProcessedCount = 0;
	for (int32 QueueIndex = 0; QueueIndex < ZeroInDegreeQueue.Num(); ++QueueIndex)
	{
		const FGameplayTag CurrentTag = ZeroInDegreeQueue[QueueIndex];
		ProcessedCount += 1;

		const TArray<FGameplayTag>* Children = ChildrenByParentTag.Find(CurrentTag);
		if (!Children)
		{
			continue;
		}

		for (const FGameplayTag& ChildTag : *Children)
		{
			int32* ChildInDegree = InDegreeByTag.Find(ChildTag);
			if (!ChildInDegree)
			{
				continue;
			}

			*ChildInDegree -= 1;
			if (*ChildInDegree == 0)
			{
				ZeroInDegreeQueue.Add(ChildTag);
			}
		}
	}

	if (ProcessedCount != InDegreeByTag.Num())
	{
		Context.AddError(FText::FromString(TEXT("Skill definition collection contains a cyclic prerequisite graph. (DAG violation)")));
		bHasError = true;
	}

	return bHasError ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

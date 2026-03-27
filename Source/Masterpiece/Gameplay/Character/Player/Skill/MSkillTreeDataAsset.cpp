// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Character/Player/Skill/MSkillTreeDataAsset.h"

#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Misc/DataValidation.h"

namespace
{
bool IsValidRequiredCharacterLevel(const int32 Level)
{
	return Level == 1 || (Level > 0 && Level % 5 == 0);
}

void AddValidationError(FDataValidationContext& Context, const FString& Message)
{
	Context.AddError(FText::FromString(Message));
}
}

bool UMSkillTreeDataAsset::GetAllSkillDefinitions(TArray<const FMSkillDefinitionBase*>& OutDefinitions, TArray<bool>& OutPassiveFlags) const
{
	OutDefinitions.Reset();
	OutPassiveFlags.Reset();
	OutDefinitions.Reserve(Skills.Num());
	OutPassiveFlags.Reserve(Skills.Num());

	for (const FInstancedStruct& SkillEntry : Skills)
	{
		if (const FMSkillDefinitionActive* ActiveDefinition = SkillEntry.GetPtr<FMSkillDefinitionActive>())
		{
			OutDefinitions.Add(ActiveDefinition);
			OutPassiveFlags.Add(false);
			continue;
		}

		if (const FMSkillDefinitionPassive* PassiveDefinition = SkillEntry.GetPtr<FMSkillDefinitionPassive>())
		{
			OutDefinitions.Add(PassiveDefinition);
			OutPassiveFlags.Add(true);
			continue;
		}

		return false;
	}

	return true;
}

#if WITH_EDITOR
EDataValidationResult UMSkillTreeDataAsset::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	bool bHasError = false;

	TArray<const FMSkillDefinitionBase*> Definitions;
	TArray<bool> PassiveFlags;
	if (!GetAllSkillDefinitions(Definitions, PassiveFlags))
	{
		AddValidationError(Context, TEXT("지원하지 않는 스킬 정의 타입이 포함되어 있습니다."));
		bHasError = true;
		return EDataValidationResult::Invalid;
	}

	TMap<FGameplayTag, int32> SkillIndexByTag;
	TMap<FGameplayTag, int32> SkillLevelByTag;
	SkillIndexByTag.Reserve(Definitions.Num());
	SkillLevelByTag.Reserve(Definitions.Num());

	for (int32 Index = 0; Index < Definitions.Num(); ++Index)
	{
		const FMSkillDefinitionBase* Definition = Definitions[Index];
		if (!Definition)
		{
			AddValidationError(Context, FString::Printf(TEXT("Index %d: 스킬 정의가 비어 있습니다."), Index));
			bHasError = true;
			continue;
		}

		if (!Definition->SkillTag.IsValid())
		{
			AddValidationError(Context, FString::Printf(TEXT("Index %d: SkillTag가 유효하지 않습니다."), Index));
			bHasError = true;
			continue;
		}

		if (SkillIndexByTag.Contains(Definition->SkillTag))
		{
			AddValidationError(Context, FString::Printf(TEXT("중복 SkillTag가 있습니다: %s"), *Definition->SkillTag.ToString()));
			bHasError = true;
		}
		else
		{
			SkillIndexByTag.Add(Definition->SkillTag, Index);
			SkillLevelByTag.Add(Definition->SkillTag, Definition->RequiredCharacterLevel);
		}

		if (!IsValidRequiredCharacterLevel(Definition->RequiredCharacterLevel))
		{
			AddValidationError(Context, FString::Printf(TEXT("%s: RequiredCharacterLevel은 1 또는 5의 배수여야 합니다. 현재값=%d"), *Definition->SkillTag.ToString(), Definition->RequiredCharacterLevel));
			bHasError = true;
		}
	}

	TMap<FGameplayTag, int32> InDegreeByTag;
	TMap<FGameplayTag, TArray<FGameplayTag>> ChildrenByParentTag;
	for (const TPair<FGameplayTag, int32>& Pair : SkillIndexByTag)
	{
		InDegreeByTag.Add(Pair.Key, 0);
	}

	for (const FMSkillDefinitionBase* Definition : Definitions)
	{
		if (!Definition || !Definition->SkillTag.IsValid())
		{
			continue;
		}

		for (const FMSkillPrerequisite& Prerequisite : Definition->Prerequisites)
		{
			if (!Prerequisite.SkillTag.IsValid())
			{
				AddValidationError(Context, FString::Printf(TEXT("%s: Prerequisite SkillTag가 유효하지 않습니다."), *Definition->SkillTag.ToString()));
				bHasError = true;
				continue;
			}

			const int32* ParentIndex = SkillIndexByTag.Find(Prerequisite.SkillTag);
			if (!ParentIndex)
			{
				AddValidationError(Context, FString::Printf(TEXT("%s: 존재하지 않는 부모 스킬 참조: %s"), *Definition->SkillTag.ToString(), *Prerequisite.SkillTag.ToString()));
				bHasError = true;
				continue;
			}

			const int32* ParentLevel = SkillLevelByTag.Find(Prerequisite.SkillTag);
			const int32* ChildLevel = SkillLevelByTag.Find(Definition->SkillTag);
			if (ParentLevel && ChildLevel && !(*ParentLevel < *ChildLevel))
			{
				AddValidationError(Context, FString::Printf(TEXT("%s: 부모(%s) RequiredCharacterLevel(%d)은 자식 레벨(%d)보다 반드시 작아야 합니다."),
					*Definition->SkillTag.ToString(), *Prerequisite.SkillTag.ToString(), *ParentLevel, *ChildLevel));
				bHasError = true;
			}

			InDegreeByTag.FindOrAdd(Definition->SkillTag) += 1;
			ChildrenByParentTag.FindOrAdd(Prerequisite.SkillTag).Add(Definition->SkillTag);
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
		AddValidationError(Context, TEXT("스킬 트리에 순환 참조가 있습니다. (DAG 위반)"));
		bHasError = true;
	}

	return bHasError ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "MSkillTreeDataAsset.generated.h"

struct FMSkillDefinitionBase;
struct FInstancedStruct;
UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillTreeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill", meta=(BaseStruct="/Script/Masterpiece.MSkillDefinitionBase", ExcludeBaseStruct))
	TArray<FInstancedStruct> Skills;

	bool GetAllSkillDefinitions(TArray<const FMSkillDefinitionBase*>& OutDefinitions, TArray<bool>& OutPassiveFlags) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};

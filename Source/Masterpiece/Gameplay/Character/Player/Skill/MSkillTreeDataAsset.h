// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "MSkillTreeDataAsset.generated.h"

struct FInstancedStruct;

UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillTreeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill", meta=(BaseStruct="/Script/Masterpiece.MSkillDefinitionBase", ExcludeBaseStruct))
	TArray<FInstancedStruct> Skills;
};

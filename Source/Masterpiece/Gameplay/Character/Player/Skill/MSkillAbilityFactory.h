// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MSkillAbilityFactory.generated.h"

struct FGameplayAbilitySpec;
struct FMSkillDefinitionActive;
struct FGameplayTag;

UCLASS()
class MASTERPIECE_API UMSkillAbilityFactory : public UObject
{
	GENERATED_BODY()

public:
	static bool BuildAbilitySpec(const FMSkillDefinitionActive& SkillDefinition, const FGameplayTag& SlotTag, int32 SkillRank, UObject* SourceObject, FGameplayAbilitySpec& OutSpec);
};

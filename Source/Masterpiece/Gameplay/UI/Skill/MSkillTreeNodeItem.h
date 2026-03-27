// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "MSkillTreeNodeItem.generated.h"

UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillTreeNodeItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FGameplayTag SkillTag;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 RequiredCharacterLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 CostPerRank = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 MaxRank = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	TArray<FGameplayTag> ParentSkillTags;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	TArray<FGameplayTag> ChildSkillTags;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 LayerIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 ColumnIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FVector2D GraphPosition = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	bool bPassive = false;
};

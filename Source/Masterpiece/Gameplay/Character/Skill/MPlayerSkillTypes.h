// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerSkillTypes.generated.h"

UENUM(BlueprintType)
enum class EMPlayerSkillType : uint8
{
	SkillQ,
	SkillW,
	SkillE,
	SkillR,
	MAX UMETA(Hidden)
};

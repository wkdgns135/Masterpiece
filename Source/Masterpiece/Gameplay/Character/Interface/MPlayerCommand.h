// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MPlayerCommand.generated.h"

UENUM(BlueprintType)
enum class EMSkillSlot : uint8
{
	SkillQ,
	SkillW,
	SkillE,
	SkillR,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMQuickSlot : uint8
{
	Slot1,
	Slot2,
	Slot3,
	Slot4,
	MAX UMETA(Hidden)
};

UINTERFACE(MinimalAPI, NotBlueprintable)
class UMPlayerCommand : public UInterface
{
	GENERATED_BODY()
};

class MASTERPIECE_API IMPlayerCommand
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="PlayerCommand")
	virtual void TriggerPrimaryAttack() = 0;

	UFUNCTION(BlueprintCallable, Category="PlayerCommand")
	virtual void TriggerMoveCommand() = 0;

	UFUNCTION(BlueprintCallable, Category="PlayerCommand")
	virtual void TriggerInteraction() = 0;

	UFUNCTION(BlueprintCallable, Category="PlayerCommand")
	virtual void TriggerDodge() = 0;

	UFUNCTION(BlueprintCallable, Category="PlayerCommand")
	virtual void TriggerSkill(EMSkillSlot SkillSlot) = 0;

	UFUNCTION(BlueprintCallable, Category="PlayerCommand")
	virtual void TriggerQuickSlot(EMQuickSlot QuickSlot) = 0;
};

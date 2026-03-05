// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/Skill/MSkillBase.h"
#include "MPlayerSkillBase.generated.h"

class AMPlayerCharacterBase;
class UMCombatComponent;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMPlayerSkillBase : public UMSkillBase
{
	GENERATED_BODY()

public:
	virtual bool CanExecute_Implementation(const UMCombatComponent* CombatComponent) const override;

protected:
	AMPlayerCharacterBase* ResolvePlayerCharacter(const UMCombatComponent* CombatComponent) const;
};

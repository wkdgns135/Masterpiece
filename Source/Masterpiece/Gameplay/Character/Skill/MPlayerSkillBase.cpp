// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerSkillBase.h"

#include "Gameplay/Character/Component/MCombatComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"


bool UMPlayerSkillBase::CanExecute_Implementation(const UMCombatComponent* CombatComponent) const
{
	if (!Super::CanExecute_Implementation(CombatComponent))
	{
		return false;
	}

	return IsValid(ResolvePlayerCharacter(CombatComponent));
}

AMPlayerCharacterBase* UMPlayerSkillBase::ResolvePlayerCharacter(const UMCombatComponent* CombatComponent) const
{
	if (!IsValid(CombatComponent))
	{
		return nullptr;
	}

	return Cast<AMPlayerCharacterBase>(CombatComponent->GetOwnerCharacter());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MSkillBase.h"

#include "Gameplay/Character/Component/MCombatComponent.h"


bool UMSkillBase::CanExecute_Implementation(const UMCombatComponent* CombatComponent) const
{
	if (!IsValid(CombatComponent))
	{
		return false;
	}

	return !CombatComponent->IsSkillOnCooldown(GetSkillId(), GetCooldownSeconds());
}

void UMSkillBase::Execute_Implementation(UMCombatComponent* CombatComponent)
{
}

FName UMSkillBase::GetSkillId() const
{
	return SkillId;
}

float UMSkillBase::GetCooldownSeconds() const
{
	return CooldownSeconds;
}

float UMSkillBase::GetSkillRange() const
{
	return SkillRange;
}

float UMSkillBase::GetDamageMultiplier() const
{
	return DamageMultiplier;
}

UAnimMontage* UMSkillBase::GetSkillMontage() const
{
	return SkillMontage;
}

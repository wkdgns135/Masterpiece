// Fill out your copyright notice in the Description page of Project Settings.

#include "MAbilityCost.h"

bool UMAbilityCost::CheckCost_Implementation(const UMGameplayAbility* Ability) const
{
	return Ability != nullptr;
}

void UMAbilityCost::ApplyCost_Implementation(const UMGameplayAbility* Ability) const
{
	
}

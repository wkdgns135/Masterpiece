// Fill out your copyright notice in the Description page of Project Settings.


#include "MAbilitySystemInterface.h"

#include "GameplayTagContainer.h"
#include "MAbilitySystemComponent.h"

bool IMAbilitySystemInterface::TryActivateAbilityByTag(const FGameplayTag& AbilityTag) const
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	UMAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);
	return AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer, true);
}

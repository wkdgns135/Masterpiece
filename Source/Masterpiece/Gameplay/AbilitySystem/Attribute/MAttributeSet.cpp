// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeSet.h"

#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"

UMAbilitySystemComponent* UMAttributeSet::GetAbilitySystemComponent() const
{
	return Cast<UMAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

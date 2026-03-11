// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeSet.h"

UMAttributeSet::UMAttributeSet()
{
}

UMAbilitySystemComponent* UMAttributeSet::GetMAbilitySystemComponent() const
{
	return Cast<UMAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

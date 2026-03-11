// Fill out your copyright notice in the Description page of Project Settings.


#include "MAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UMAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);
	
	GameplayTagPropertyMap.Initialize(this, AbilitySystemComponent);
}

void UMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (const AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(AbilitySystemComponent);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayPlayerState.h"

#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Attribute/MAttributeSet.h"

AMGameplayPlayerState::AMGameplayPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMAttributeSet>(TEXT("AttributeSet"));
}

UMAbilitySystemComponent* AMGameplayPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMAttributeSet* AMGameplayPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool AMGameplayPlayerState::AreStartupAbilitiesGranted() const
{
	return bStartupAbilitiesGranted;
}

void AMGameplayPlayerState::SetStartupAbilitiesGranted(const bool bGranted)
{
	bStartupAbilitiesGranted = bGranted;
}

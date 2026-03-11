// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayPlayerState.h"

#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/AbilitySystem/Attribute/MPlayerAttributeSet.h"

AMGameplayPlayerState::AMGameplayPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CombatAttributeSet = CreateDefaultSubobject<UMCombatAttributeSet>(TEXT("CombatAttributeSet"));
	PlayerAttributeSet = CreateDefaultSubobject<UMPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

UMAbilitySystemComponent* AMGameplayPlayerState::GetMAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* AMGameplayPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMCombatAttributeSet* AMGameplayPlayerState::GetCombatAttributeSet() const
{
	return CombatAttributeSet;
}

UMPlayerAttributeSet* AMGameplayPlayerState::GetPlayerAttributeSet() const
{
	return PlayerAttributeSet;
}

bool AMGameplayPlayerState::AreStartupAbilitiesGranted() const
{
	return bStartupAbilitiesGranted;
}

void AMGameplayPlayerState::SetStartupAbilitiesGranted(const bool bGranted)
{
	bStartupAbilitiesGranted = bGranted;
}

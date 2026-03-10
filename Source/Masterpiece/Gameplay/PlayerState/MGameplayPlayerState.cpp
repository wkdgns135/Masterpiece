// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayPlayerState.h"

#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"

AMGameplayPlayerState::AMGameplayPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
}

UMAbilitySystemComponent* AMGameplayPlayerState::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}

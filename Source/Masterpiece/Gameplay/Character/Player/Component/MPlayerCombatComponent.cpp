// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

UMPlayerCombatComponent::UMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

float UMPlayerCombatComponent::GetPlayerAttackRange() const
{
	return GetAttackRange();
}

float UMPlayerCombatComponent::GetPlayerAttackSpeed() const
{
	return GetAttackSpeed();
}

void UMPlayerCombatComponent::ExecuteDodge()
{
	
}

void UMPlayerCombatComponent::ExecuteInteraction()
{
	
}

void UMPlayerCombatComponent::ExecuteSkillSlot(const int32 SkillSlotIndex)
{
}

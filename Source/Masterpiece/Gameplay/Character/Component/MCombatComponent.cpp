// Fill out your copyright notice in the Description page of Project Settings.


#include "MCombatComponent.h"

#include "Gameplay/Character/MCharacterBase.h"

UMCombatComponent::UMCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AMCharacterBase>(GetOwner());
}

AMCharacterBase* UMCombatComponent::GetOwnerCharacter() const
{
	return OwnerCharacter;
}

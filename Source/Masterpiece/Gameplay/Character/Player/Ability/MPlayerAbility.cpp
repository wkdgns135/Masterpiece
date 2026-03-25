// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerCombatComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"

AMPlayerCharacterBase* UMPlayerAbility::GetMPlayerCharacter() const
{
	return Cast<AMPlayerCharacterBase>(GetAvatarActorFromActorInfo());
}

AMGameplayPlayerController* UMPlayerAbility::GetMGameplayPlayerController() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	return PlayerCharacter ? Cast<AMGameplayPlayerController>(PlayerCharacter->GetController()) : nullptr;
}

UMPlayerMovementComponent* UMPlayerAbility::GetMPlayerMovementComponent() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	return PlayerCharacter ? PlayerCharacter->GetPlayerMovementComponent() : nullptr;
}

UMPlayerCombatComponent* UMPlayerAbility::GetMPlayerCombatComponent() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	return PlayerCharacter ? PlayerCharacter->GetPlayerCombatComponent() : nullptr;
}

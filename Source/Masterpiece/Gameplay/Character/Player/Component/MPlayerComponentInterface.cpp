// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerComponentInterface.h"

#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"
#include "Gameplay/PlayerState/MGameplayPlayerState.h"

AMPlayerCharacterBase* IMPlayerComponentInterface::GetMPlayerCharacter() const
{
	if (const UActorComponent* Component = Cast<UActorComponent>(this))
	{
		return Cast<AMPlayerCharacterBase>(Component->GetOwner());
	}
	return nullptr;
}

AMGameplayPlayerController* IMPlayerComponentInterface::GetMGameplayPlayerController() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	return PlayerCharacter ? Cast<AMGameplayPlayerController>(PlayerCharacter->GetController()) : nullptr;
}

AMGameplayPlayerState* IMPlayerComponentInterface::GetMGameplayPlayerState() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	return PlayerCharacter ? PlayerCharacter->GetPlayerState<AMGameplayPlayerState>() : nullptr;
}

UMPlayerInputComponent* IMPlayerComponentInterface::GetMPlayerInputComponent() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	return PlayerCharacter ? PlayerCharacter->GetPlayerInputComponent() : nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayGameMode.h"

#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"
#include "Gameplay/PlayerState/MGameplayPlayerState.h"

AMGameplayGameMode::AMGameplayGameMode()
{
	PlayerControllerClass = AMGameplayPlayerController::StaticClass();
	DefaultPawnClass = AMPlayerCharacterBase::StaticClass();
	PlayerStateClass = AMGameplayPlayerState::StaticClass();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayGameMode.h"

#include "Gameplay/PlayerController/MGameplayPlayerController.h"

AMGameplayGameMode::AMGameplayGameMode()
{
	PlayerControllerClass = AMGameplayPlayerController::StaticClass();
}

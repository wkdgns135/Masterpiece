// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayPlayerController.h"

#include "Gameplay/Character/Player/Input/MPlayerInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMGameplayPlayerController::AMGameplayPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AMGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

bool AMGameplayPlayerController::TraceCursorToWorld(FHitResult& OutHitResult) const
{
	return GetHitResultUnderCursor(ECC_Visibility, true, OutHitResult);
}

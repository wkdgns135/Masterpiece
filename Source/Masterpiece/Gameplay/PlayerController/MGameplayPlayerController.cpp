// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayPlayerController.h"

#include "EnhancedInputSubsystems.h"

void AMGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// add the input mapping context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}

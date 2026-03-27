// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayPlayerController.h"

#include "Engine/LocalPlayer.h"
#include "Gameplay/UI/MUIManagerSubsystem.h"
#include "Gameplay/UI/MUIGameplayTags.h"

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

bool AMGameplayPlayerController::ToggleSkillTreeMenu()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UMUIManagerSubsystem* UIManager = LocalPlayer ? LocalPlayer->GetSubsystem<UMUIManagerSubsystem>() : nullptr;
	if (!UIManager)
	{
		return false;
	}

	return UIManager->ToggleWidgetByTag(MUIGameplayTags::UI_Widget_GameMenu_SkillTree);
}

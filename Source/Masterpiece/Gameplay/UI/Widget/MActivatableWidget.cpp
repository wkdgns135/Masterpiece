// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/Widget/MActivatableWidget.h"

#include "GameFramework/PlayerController.h"
#include "Gameplay/UI/MUIGameplayTags.h"

UMActivatableWidget::UMActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMActivatableWidget::ActivateWidget()
{
	ApplyInputPolicy();
	K2_OnWidgetActivated();
}

void UMActivatableWidget::DeactivateWidget()
{
	K2_OnWidgetDeactivated();
}

bool UMActivatableWidget::CanHandleEscape() const
{
	return bCloseOnEscape;
}

bool UMActivatableWidget::HandleEscapeRequested_Implementation()
{
	return bCloseOnEscape;
}

int32 UMActivatableWidget::GetDesiredZOrder() const
{
	return MUIGameplayTags::GetWidgetBaseZOrder(GetWidgetTag()) + ZOrderBias;
}

void UMActivatableWidget::ActivateStackWidget()
{
	ActivateWidget();
}

void UMActivatableWidget::DeactivateStackWidget()
{
	DeactivateWidget();
}

bool UMActivatableWidget::CanHandleStackEscape() const
{
	return CanHandleEscape();
}

bool UMActivatableWidget::HandleStackEscapeRequested()
{
	return HandleEscapeRequested();
}

int32 UMActivatableWidget::GetStackWidgetZOrder() const
{
	return GetDesiredZOrder();
}

void UMActivatableWidget::ApplyInputPolicy() const
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	switch (InputPolicy)
	{
	case EMUIInputPolicy::Game:
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
		}
		break;
	case EMUIInputPolicy::GameAndMenu:
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
		break;
	case EMUIInputPolicy::Menu:
		{
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
		break;
	default:
		break;
	}
}

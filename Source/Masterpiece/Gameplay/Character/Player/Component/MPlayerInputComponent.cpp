// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerInputComponent.h"

#include "EnhancedInputComponent.h"
#include "Gameplay/Character/Interface/MPlayerCommand.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerCameraComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
#include "Gameplay/Character/Player/Input/MPlayerInputConfig.h"

UMPlayerInputComponent::UMPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerInputComponent::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	check(EnhancedInputComponent);

	if (!InputConfig)
	{
		return;
	}

	if (InputConfig->MoveCommandAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->MoveCommandAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveCommandTriggered);
	}

	if (InputConfig->CursorAimAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->CursorAimAction, ETriggerEvent::Triggered, this, &ThisClass::HandleCursorAimTriggered);
	}

	if (InputConfig->PrimaryAttackAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->PrimaryAttackAction, ETriggerEvent::Started, this, &ThisClass::HandlePrimaryAttackTriggered);
	}

	if (InputConfig->ZoomAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::HandleZoomTriggered);
	}

	if (InputConfig->InteractionAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->InteractionAction, ETriggerEvent::Started, this, &ThisClass::HandleInteractionTriggered);
	}

	if (InputConfig->DodgeAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->DodgeAction, ETriggerEvent::Started, this, &ThisClass::HandleDodgeTriggered);
	}

	if (InputConfig->SkillSlotAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->SkillSlotAction, ETriggerEvent::Started, this, &ThisClass::HandleSkillSlotTriggered);
	}

	if (InputConfig->QuickSlotAction)
	{
		EnhancedInputComponent->BindAction(InputConfig->QuickSlotAction, ETriggerEvent::Started, this, &ThisClass::HandleQuickSlotTriggered);
	}
}

void UMPlayerInputComponent::HandleMoveCommandTriggered(const FInputActionValue& Value)
{
	AMPlayerCharacterBase* Character = GetPlayerCharacter();
	Character->GetPlayerMovement()->HandleMoveCommand(Value);

	if (IMPlayerCommand* PlayerCommand = Cast<IMPlayerCommand>(Character))
	{
		PlayerCommand->TriggerMoveCommand();
	}
}

void UMPlayerInputComponent::HandleCursorAimTriggered(const FInputActionValue& Value)
{
	GetPlayerCharacter()->GetPlayerMovement()->HandleCursorAim(Value);
}

void UMPlayerInputComponent::HandlePrimaryAttackTriggered(const FInputActionValue& Value)
{
	if (IMPlayerCommand* PlayerCommand = Cast<IMPlayerCommand>(GetPlayerCharacter()))
	{
		PlayerCommand->TriggerPrimaryAttack();
	}
}

void UMPlayerInputComponent::HandleZoomTriggered(const FInputActionValue& Value)
{
	GetPlayerCharacter()->GetPlayerCamera()->HandleZoomInput(Value.Get<float>());
}

void UMPlayerInputComponent::HandleInteractionTriggered(const FInputActionValue& Value)
{
	if (IMPlayerCommand* PlayerCommand = Cast<IMPlayerCommand>(GetPlayerCharacter()))
	{
		PlayerCommand->TriggerInteraction();
	}
}

void UMPlayerInputComponent::HandleDodgeTriggered(const FInputActionValue& Value)
{
	if (IMPlayerCommand* PlayerCommand = Cast<IMPlayerCommand>(GetPlayerCharacter()))
	{
		PlayerCommand->TriggerDodge();
	}
}

void UMPlayerInputComponent::HandleSkillSlotTriggered(const FInputActionValue& Value)
{
	if (IMPlayerCommand* PlayerCommand = Cast<IMPlayerCommand>(GetPlayerCharacter()))
	{
		PlayerCommand->TriggerSkill(ToSkillSlot(Value.Get<float>()));
	}
}

void UMPlayerInputComponent::HandleQuickSlotTriggered(const FInputActionValue& Value)
{
	if (IMPlayerCommand* PlayerCommand = Cast<IMPlayerCommand>(GetPlayerCharacter()))
	{
		PlayerCommand->TriggerQuickSlot(ToQuickSlot(Value.Get<float>()));
	}
}

EMSkillSlot UMPlayerInputComponent::ToSkillSlot(const float InputValue)
{
	const int32 SlotIndex = FMath::Clamp(FMath::RoundToInt(InputValue) - 1, 0, static_cast<int32>(EMSkillSlot::MAX) - 1);
	return static_cast<EMSkillSlot>(SlotIndex);
}

EMQuickSlot UMPlayerInputComponent::ToQuickSlot(const float InputValue)
{
	const int32 SlotIndex = FMath::Clamp(FMath::RoundToInt(InputValue) - 1, 0, static_cast<int32>(EMQuickSlot::MAX) - 1);
	return static_cast<EMQuickSlot>(SlotIndex);
}

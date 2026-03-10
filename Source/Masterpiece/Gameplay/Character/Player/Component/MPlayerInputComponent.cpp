// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerInputComponent.h"

#include "EnhancedInputComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Input/MPlayerInputConfig.h"

UMPlayerInputComponent::UMPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwner());
	check(PlayerCharacter);
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
	MoveCommandTriggered.Broadcast(Value);
}

void UMPlayerInputComponent::HandleCursorAimTriggered(const FInputActionValue& Value)
{
	CursorAimTriggered.Broadcast(Value);
}

void UMPlayerInputComponent::HandlePrimaryAttackTriggered(const FInputActionValue& Value)
{
	PrimaryAttackTriggered.Broadcast();
}

void UMPlayerInputComponent::HandleZoomTriggered(const FInputActionValue& Value)
{
	ZoomTriggered.Broadcast(Value.Get<float>());
}

void UMPlayerInputComponent::HandleInteractionTriggered(const FInputActionValue& Value)
{
	InteractionTriggered.Broadcast();
}

void UMPlayerInputComponent::HandleDodgeTriggered(const FInputActionValue& Value)
{
	DodgeTriggered.Broadcast();
}

void UMPlayerInputComponent::HandleSkillSlotTriggered(const FInputActionValue& Value)
{
	int32 SkillSlotIndex = INDEX_NONE;

	switch (Value.GetValueType())
	{
	case EInputActionValueType::Boolean:
		SkillSlotIndex = Value.Get<bool>() ? 0 : INDEX_NONE;
		break;
	case EInputActionValueType::Axis1D:
		SkillSlotIndex = FMath::RoundToInt(Value.Get<float>());
		break;
	case EInputActionValueType::Axis2D:
		SkillSlotIndex = FMath::RoundToInt(Value.Get<FVector2D>().X);
		break;
	case EInputActionValueType::Axis3D:
		SkillSlotIndex = FMath::RoundToInt(Value.Get<FVector>().X);
		break;
	default:
		break;
	}

	SkillSlotTriggered.Broadcast(SkillSlotIndex);
}

void UMPlayerInputComponent::HandleQuickSlotTriggered(const FInputActionValue& Value)
{
}

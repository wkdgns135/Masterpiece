// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerInputComponent.h"

#include "EnhancedInputComponent.h"
#include "Gameplay/Character/Player/Interface/MAttacker.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"

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

	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJump);
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}

	if (MouseLookAction)
	{
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}

	if (ComboAttackAction)
	{
		EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Started, this, &ThisClass::ComboAttackPressed);
	}

	if (ChargedAttackAction)
	{
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Started, this, &ThisClass::ChargedAttackStarted);
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Completed, this, &ThisClass::ChargedAttackReleased);
	}
}

void UMPlayerInputComponent::Jump(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* Character = GetPlayerCharacter())
	{
		if (UMPlayerMovementComponent* MovementComponent = Character->GetPlayerMovement())
		{
			MovementComponent->Jump(Value);
		}
	}
}

void UMPlayerInputComponent::StopJump(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* Character = GetPlayerCharacter())
	{
		if (UMPlayerMovementComponent* MovementComponent = Character->GetPlayerMovement())
		{
			MovementComponent->StopJump(Value);
		}
	}
}

void UMPlayerInputComponent::Move(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* Character = GetPlayerCharacter())
	{
		if (UMPlayerMovementComponent* MovementComponent = Character->GetPlayerMovement())
		{
			MovementComponent->Move(Value);
		}
	}
}

void UMPlayerInputComponent::Look(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* Character = GetPlayerCharacter())
	{
		if (UMPlayerMovementComponent* MovementComponent = Character->GetPlayerMovement())
		{
			MovementComponent->Look(Value);
		}
	}
}

void UMPlayerInputComponent::ComboAttackPressed(const FInputActionValue& Value)
{
	if (IMAttacker* Attacker = Cast<IMAttacker>(GetPlayerCharacter()))
	{
		Attacker->CheckCombo();
	}
}

void UMPlayerInputComponent::ChargedAttackStarted(const FInputActionValue& Value)
{
	if (IMAttacker* Attacker = Cast<IMAttacker>(GetPlayerCharacter()))
	{
		Attacker->CheckChargedAttack();
	}
}

void UMPlayerInputComponent::ChargedAttackReleased(const FInputActionValue& Value)
{
}


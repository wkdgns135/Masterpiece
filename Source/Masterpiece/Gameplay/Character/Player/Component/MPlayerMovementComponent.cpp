// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"

#include "InputActionValue.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UMPlayerMovementComponent::UMPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMPlayerMovementComponent::Jump(const FInputActionValue& Value)
{
	AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	PlayerCharacter->Jump();
}

void UMPlayerMovementComponent::StopJump(const FInputActionValue& Value)
{
	AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	PlayerCharacter->StopJumping();
}

void UMPlayerMovementComponent::Move(const FInputActionValue& Value)
{
	AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.IsNearlyZero())
	{
		return;
	}

	const FRotator ControlRotation = PlayerCharacter->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void UMPlayerMovementComponent::Look(const FInputActionValue& Value)
{
	AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

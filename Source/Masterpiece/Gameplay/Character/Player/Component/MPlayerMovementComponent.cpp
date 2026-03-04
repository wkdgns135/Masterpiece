// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "InputActionValue.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"

UMPlayerMovementComponent::UMPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerMovementComponent::HandleMoveCommand(const FInputActionValue& Value)
{
	IssueMoveToCursorCommand();
}

void UMPlayerMovementComponent::HandleCursorAim(const FInputActionValue& Value)
{
	FaceCursorDirection();
}

void UMPlayerMovementComponent::IssueMoveToCursorCommand()
{
	AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	AMGameplayPlayerController* PlayerController = Cast<AMGameplayPlayerController>(PlayerCharacter->GetController());
	if (!PlayerController)
	{
		return;
	}

	FHitResult CursorHit;
	if (!PlayerController->TraceCursorToWorld(CursorHit))
	{
		return;
	}

	FVector Destination = CursorHit.ImpactPoint;
	Destination.Z = PlayerCharacter->GetActorLocation().Z;

	FaceCursorDirection();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, Destination);
}

void UMPlayerMovementComponent::FaceCursorDirection()
{
	AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	AMGameplayPlayerController* PlayerController = Cast<AMGameplayPlayerController>(PlayerCharacter->GetController());
	if (!PlayerController)
	{
		return;
	}

	FHitResult CursorHit;
	if (!PlayerController->TraceCursorToWorld(CursorHit))
	{
		return;
	}

	FVector Direction = CursorHit.ImpactPoint - PlayerCharacter->GetActorLocation();
	Direction.Z = 0.0f;
	if (!Direction.IsNearlyZero())
	{
		PlayerCharacter->SetActorRotation(Direction.Rotation());
	}
}

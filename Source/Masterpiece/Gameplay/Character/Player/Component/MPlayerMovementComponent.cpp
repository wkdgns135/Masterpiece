// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"

UMPlayerMovementComponent::UMPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwner());
	check(PlayerCharacter);

	UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent();
	check(InputComponent);

	MoveCommandDelegateHandle = InputComponent->OnMoveCommandTriggered().AddUObject(this, &ThisClass::HandleMoveCommand);
	CursorAimDelegateHandle = InputComponent->OnCursorAimTriggered().AddUObject(this, &ThisClass::HandleCursorAim);
}

void UMPlayerMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerCharacter)
	{
		if (UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent())
		{
			InputComponent->OnMoveCommandTriggered().Remove(MoveCommandDelegateHandle);
			InputComponent->OnCursorAimTriggered().Remove(CursorAimDelegateHandle);
		}
	}

	Super::EndPlay(EndPlayReason);
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

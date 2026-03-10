// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"
#include "TimerManager.h"

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
	StopRotationInterpolationTimer();

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
		const FRotator NewDesiredRotation = Direction.Rotation();
		const float DeltaYawAbs = FMath::Abs(FMath::FindDeltaAngleDegrees(PlayerCharacter->GetActorRotation().Yaw, NewDesiredRotation.Yaw));
		if (DeltaYawAbs < RotationSettings.RotationStartThresholdDeg)
		{
			return;
		}

		DesiredRotation = NewDesiredRotation;
		bHasDesiredRotation = true;
		StartRotationInterpolationTimer();
	}
}

void UMPlayerMovementComponent::StartRotationInterpolationTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!World->GetTimerManager().IsTimerActive(RotationTimerHandle))
	{
		LastRotationUpdateTimeSec = World->GetTimeSeconds();
		World->GetTimerManager().SetTimer(
			RotationTimerHandle,
			this,
			&ThisClass::UpdateRotationWithTimer,
			RotationSettings.RotationUpdateIntervalSec,
			true);
	}
}

void UMPlayerMovementComponent::StopRotationInterpolationTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RotationTimerHandle);
	}

	bHasDesiredRotation = false;
}

void UMPlayerMovementComponent::UpdateRotationWithTimer()
{
	if (!PlayerCharacter || !bHasDesiredRotation)
	{
		StopRotationInterpolationTimer();
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		StopRotationInterpolationTimer();
		return;
	}

	const float CurrentTimeSec = World->GetTimeSeconds();
	const float DeltaTimeSec = FMath::Max(CurrentTimeSec - LastRotationUpdateTimeSec, KINDA_SMALL_NUMBER);
	LastRotationUpdateTimeSec = CurrentTimeSec;

	const float CurrentYaw = PlayerCharacter->GetActorRotation().Yaw;
	const float TargetYaw = DesiredRotation.Yaw;
	const float DeltaYawAbs = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw));

	if (DeltaYawAbs <= RotationSettings.RotationCompleteThresholdDeg)
	{
		PlayerCharacter->SetActorRotation(DesiredRotation);
		StopRotationInterpolationTimer();
		return;
	}

	const float InterpolatedSpeed = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, 180.0f),
		FVector2D(RotationSettings.MinRotationSpeedDegPerSec, RotationSettings.MaxRotationSpeedDegPerSec),
		DeltaYawAbs);
	const float MaxStep = InterpolatedSpeed * DeltaTimeSec;
	const float NewYaw = FMath::FixedTurn(CurrentYaw, TargetYaw, MaxStep);

	FRotator NewRotation = PlayerCharacter->GetActorRotation();
	NewRotation.Yaw = NewYaw;
	PlayerCharacter->SetActorRotation(NewRotation);
}

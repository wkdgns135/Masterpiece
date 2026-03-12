// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Controller.h"
#include "Gameplay/Interface/MDamageable.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Input/MPlayerInputComponent.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"
#include "TimerManager.h"

void UMPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopRotationInterpolationTimer();
	Super::EndPlay(EndPlayReason);
}

void UMPlayerMovementComponent::DoMoveToCursor()
{
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController();
	if (!PlayerCharacter || !PlayerController)
	{
		return;
	}


	FHitResult CursorHit;
	if (!PlayerController->TraceCursorToWorld(CursorHit))
	{
		return;
	}

	if (const AActor* HitActor = CursorHit.GetActor(); IsValid(HitActor) && HitActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		return;
	}

	FVector Destination = CursorHit.ImpactPoint;
	Destination.Z = PlayerCharacter->GetActorLocation().Z;

	FaceCursorDirection();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, Destination);
}

void UMPlayerMovementComponent::DoMoveToTargetActor(AActor* TargetActor)
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!PlayerCharacter || !IsValid(TargetActor))
	{
		return;
	}

	AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController();
	if (!PlayerController)
	{
		return;
	}

	FaceCursorDirection();
	UAIBlueprintHelperLibrary::SimpleMoveToActor(PlayerController, TargetActor);
}

void UMPlayerMovementComponent::FaceCursorDirection()
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	const AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController();
	if (!PlayerCharacter || !PlayerController)
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
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
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

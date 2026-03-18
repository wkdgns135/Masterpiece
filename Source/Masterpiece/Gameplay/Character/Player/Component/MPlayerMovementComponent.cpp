// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Controller.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Input/MPlayerInputComponent.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"
#include "TimerManager.h"

UMPlayerMovementComponent::UMPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsNavigating)
	{
		UpdateNavigation();
	}
	
	if (bIsRotating)
	{
		UpdateRotation();
	}
}

void UMPlayerMovementComponent::DoMoveToActor(const AActor* TargetActor, const float AcceptanceRadius)
{
	CurrentTargetActor = MakeWeakObjectPtr(TargetActor);
	CurrentAcceptanceRadius = AcceptanceRadius;
	bIsNavigating = true;
}

void UMPlayerMovementComponent::DoMoveToLocation(const FVector &Destination)
{
	CurrentTargetLocation = Destination;
	CurrentAcceptanceRadius = DefaultAcceptanceRadius;
	bIsNavigating = true;
}

void UMPlayerMovementComponent::StopNavigationMovement()
{
	CurrentTargetActor.Reset();
	bIsNavigating = false;
	if (AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController())
	{
		PlayerController->StopMovement();
	}
	OnNavigationMoveFinishedDelegate.Broadcast(false);
}

void UMPlayerMovementComponent::FaceTargetLocation(const FVector& TargetLocation)
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	const AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController();
	if (!PlayerCharacter || !PlayerController)
	{
		return;
	}

	FVector Direction = TargetLocation - PlayerCharacter->GetActorLocation();
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
		bIsRotating = true;
	}
}

void UMPlayerMovementComponent::FaceTargetActor(const AActor* TargetActor)
{
	if (!IsValid(TargetActor))
	{
		return;
	}
	FaceTargetLocation(TargetActor->GetActorLocation());
}

void UMPlayerMovementComponent::UpdateNavigation()
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!PlayerCharacter)
	{
		bIsNavigating = false;
		return;
	}
	
	if (CurrentTargetActor.IsValid())
	{
		if (CheckDistanceFromLocation(CurrentTargetActor->GetActorLocation()))
		{
			bIsNavigating = false;
			OnNavigationMoveFinishedDelegate.Broadcast(true);
			CurrentTargetActor.Reset();
			return;
		}
		UAIBlueprintHelperLibrary::SimpleMoveToActor(PlayerCharacter->GetController(), CurrentTargetActor.Get());
	} 
	else
	{
		if (CheckDistanceFromLocation(CurrentTargetLocation))
		{
			bIsNavigating = false;
			OnNavigationMoveFinishedDelegate.Broadcast(true);
			return;
		}
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerCharacter->GetController(), CurrentTargetLocation);
	}
}

bool UMPlayerMovementComponent::CheckDistanceFromLocation(const FVector& TargetLocation)
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!PlayerCharacter)
	{
		return false;
	}

	const float Distance = FVector::Dist2D(PlayerCharacter->GetActorLocation(), TargetLocation);
	return Distance <= CurrentAcceptanceRadius;
}

void UMPlayerMovementComponent::UpdateRotation()
{
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!PlayerCharacter || !bHasDesiredRotation)
	{
		bIsRotating = false;
		return;
	}
	
	const float CurrentTimeSec = GetWorld()->GetTimeSeconds();
	const float DeltaTimeSec = FMath::Max(CurrentTimeSec - LastRotationUpdateTimeSec, KINDA_SMALL_NUMBER);
	LastRotationUpdateTimeSec = CurrentTimeSec;

	const float CurrentYaw = PlayerCharacter->GetActorRotation().Yaw;
	const float TargetYaw = DesiredRotation.Yaw;
	const float DeltaYawAbs = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw));

	if (DeltaYawAbs <= RotationSettings.RotationCompleteThresholdDeg)
	{
		PlayerCharacter->SetActorRotation(DesiredRotation);
		bIsRotating = false;
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

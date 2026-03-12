// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCameraComponent.h"

#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"

UMPlayerCameraComponent::UMPlayerCameraComponent()
{
}

void UMPlayerCameraComponent::HandleZoomInput(const FInputActionValue& Value)
{
	const float ZoomDelta = Value.Get<float>();
	if (!SpringArm || FMath::IsNearlyZero(ZoomDelta))
	{
		return;
	}

	const float TargetArmLength = SpringArm->TargetArmLength + (ZoomDelta * ZoomStep);
	SpringArm->TargetArmLength = FMath::Clamp(TargetArmLength, MinZoomLength, MaxZoomLength);
}

void UMPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeCameraComponents();
}

void UMPlayerCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMPlayerCameraComponent::InitializeCameraComponents()
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	check(PlayerCharacter);

	SpringArm = PlayerCharacter->GetSpringArmComponent();
	Camera = PlayerCharacter->GetFollowCameraComponent();

	check(SpringArm);
	check(Camera);
}

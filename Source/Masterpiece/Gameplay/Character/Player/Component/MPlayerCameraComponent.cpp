// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCameraComponent.h"

#include "Camera/CameraComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"

UMPlayerCameraComponent::UMPlayerCameraComponent()
{
}

void UMPlayerCameraComponent::HandleZoomInput(const float ZoomDelta)
{
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

void UMPlayerCameraComponent::InitializeCameraComponents()
{
	const AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	SpringArm = PlayerCharacter->GetSpringArm();
	Camera = PlayerCharacter->GetFollowCamera();

	check(SpringArm);
	check(Camera);
}

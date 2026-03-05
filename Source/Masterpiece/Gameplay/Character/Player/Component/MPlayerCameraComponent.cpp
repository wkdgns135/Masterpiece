// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCameraComponent.h"

#include "Camera/CameraComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"
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

	PlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwner());
	check(PlayerCharacter);

	InitializeCameraComponents();

	UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent();
	check(InputComponent);

	ZoomDelegateHandle = InputComponent->OnZoomTriggered().AddUObject(this, &ThisClass::HandleZoomInput);
}

void UMPlayerCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerCharacter)
	{
		if (UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent())
		{
			InputComponent->OnZoomTriggered().Remove(ZoomDelegateHandle);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UMPlayerCameraComponent::InitializeCameraComponents()
{
	SpringArm = PlayerCharacter->GetSpringArmComponent();
	Camera = PlayerCharacter->GetFollowCameraComponent();

	check(SpringArm);
	check(Camera);
}

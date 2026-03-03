// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCameraComponent.h"

#include "Camera/CameraComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"

UMPlayerCameraComponent::UMPlayerCameraComponent()
{
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MPlayerCameraComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMPlayerCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerCameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMPlayerCameraComponent();

	UFUNCTION()
	void HandleZoomInput(float ZoomDelta);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void InitializeCameraComponents();

private:
	UPROPERTY()
	TObjectPtr<AMPlayerCharacterBase> PlayerCharacter;

	FDelegateHandle ZoomDelegateHandle;

	UPROPERTY(EditAnywhere, Category="Camera")
	float ZoomStep = 100.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float MinZoomLength = 300.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float MaxZoomLength = 1000.0f;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;
};

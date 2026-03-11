// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MPlayerComponentInterface.h"
#include "MPlayerMovementComponent.generated.h"

struct FInputActionValue;
struct FTimerHandle;
class AMPlayerCharacterBase;

USTRUCT(BlueprintType)
struct FMRotationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float MinRotationSpeedDegPerSec = 360.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float MaxRotationSpeedDegPerSec = 1440.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float RotationCompleteThresholdDeg = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float RotationStartThresholdDeg = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float RotationUpdateIntervalSec = 0.016f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerMovementComponent : public UCharacterMovementComponent, public IMPlayerComponentInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION()
	void HandleMoveCommand(const FInputActionValue& Value);

	UFUNCTION()
	void HandleCursorAim(const FInputActionValue& Value);

	UFUNCTION()
	void IssueMoveToCursorCommand();

	UFUNCTION(BlueprintCallable, Category="Movement")
	void IssueMoveToActorCommand(AActor* TargetActor);

	UFUNCTION()
	void FaceCursorDirection();

private:
	void StartRotationInterpolationTimer();
	void StopRotationInterpolationTimer();
	void UpdateRotationWithTimer();

private:
	UPROPERTY(EditDefaultsOnly, Category="Movement|Rotation", meta=(ShowOnlyInnerProperties))
	FMRotationSettings RotationSettings;

	UPROPERTY(Transient)
	FRotator DesiredRotation = FRotator::ZeroRotator;

	UPROPERTY(Transient)
	float LastRotationUpdateTimeSec = 0.0f;

	UPROPERTY(Transient)
	bool bHasDesiredRotation = false;

	FTimerHandle RotationTimerHandle;

	FDelegateHandle MoveCommandDelegateHandle;
	FDelegateHandle CursorAimDelegateHandle;
};

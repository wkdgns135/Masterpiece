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
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNavigationMoveFinished, const bool);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerMovementComponent : public UCharacterMovementComponent, public IMPlayerComponentInterface
{
	GENERATED_BODY()

public:
	UMPlayerMovementComponent();
	
	UFUNCTION()
	void DoMoveToActor(const AActor* TargetActor, const float AcceptanceRadius = 0.0f);
	
	UFUNCTION()
	void DoMoveToLocation(const FVector& Destination);
	
	UFUNCTION()
	void StopNavigationMovement();
	
	UFUNCTION()
	void FaceCursorDirection();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 네비게이션
	void UpdateNavigation();
	bool CheckDistanceFromLocation(const FVector& TargetLocation);
	
	// 회전
	void UpdateRotation();

public:
	FOnNavigationMoveFinished OnNavigationMoveFinishedDelegate;

private:
	// 네비게이션 관련 변수
	UPROPERTY(EditDefaultsOnly, Category="Movement|Navigation")
	float DefaultAcceptanceRadius = 40.0f;
	
	UPROPERTY()
	TWeakObjectPtr<const AActor> CurrentTargetActor;
	
	bool bIsNavigating = false;
	float CurrentAcceptanceRadius = 0.0f;
	FVector CurrentTargetLocation;
	
	// 회전 관련 변수
	UPROPERTY(EditDefaultsOnly, Category="Movement|Rotation", meta=(ShowOnlyInnerProperties))
	FMRotationSettings RotationSettings;
	
	FRotator DesiredRotation = FRotator::ZeroRotator;
	float LastRotationUpdateTimeSec = 0.0f;
	bool bHasDesiredRotation = false;
	bool bIsRotating = false;
	FTimerHandle RotationTimerHandle;

};

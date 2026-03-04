// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentBase.h"
#include "MPlayerMovementComponent.generated.h"

struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerMovementComponent : public UMPlayerComponentBase
{
	GENERATED_BODY()

public:
	UMPlayerMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void HandleMoveCommand(const FInputActionValue& Value);

	UFUNCTION()
	void HandleCursorAim(const FInputActionValue& Value);

	UFUNCTION()
	void IssueMoveToCursorCommand();

	UFUNCTION()
	void FaceCursorDirection();
};

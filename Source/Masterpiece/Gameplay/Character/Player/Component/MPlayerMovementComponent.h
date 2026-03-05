// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MPlayerMovementComponent.generated.h"

struct FInputActionValue;
class AMPlayerCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerMovementComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMPlayerMovementComponent();

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

	UFUNCTION()
	void FaceCursorDirection();

private:
	UPROPERTY()
	TObjectPtr<AMPlayerCharacterBase> PlayerCharacter;

	FDelegateHandle MoveCommandDelegateHandle;
	FDelegateHandle CursorAimDelegateHandle;
};

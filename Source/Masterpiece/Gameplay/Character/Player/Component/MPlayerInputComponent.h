// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/Interface/MPlayerCommand.h"
#include "MPlayerComponentBase.h"
#include "Components/ActorComponent.h"
#include "MPlayerInputComponent.generated.h"

struct FInputActionValue;
class UEnhancedInputComponent;
class UMPlayerInputConfig;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerInputComponent : public UMPlayerComponentBase
{
	GENERATED_BODY()

public:
	UMPlayerInputComponent();

protected:
	virtual void BeginPlay() override;

public:
	void BindInputActions(UEnhancedInputComponent* EnhancedInputComponent);

	UFUNCTION()
	void HandleMoveCommandTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandleCursorAimTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandlePrimaryAttackTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandleZoomTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandleInteractionTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandleDodgeTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandleSkillSlotTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void HandleQuickSlotTriggered(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UMPlayerInputConfig> InputConfig;

private:
	static EMSkillSlot ToSkillSlot(float InputValue);
	static EMQuickSlot ToQuickSlot(float InputValue);
};

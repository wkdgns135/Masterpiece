// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MPlayerComponentInterface.h"
#include "MPlayerComponent.generated.h"

struct FInputActionValue;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerComponent : public UActorComponent, public IMPlayerComponentInterface
{
	GENERATED_BODY()

public:
	UMPlayerComponent();
	UFUNCTION(BlueprintPure, Category="Player")
	bool IsPlayerComponentInitialized() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void HandleRightClickCommand(const FInputActionValue& Value);
	virtual void HandleCursorAim(const FInputActionValue& Value);
	virtual void HandleZoom(const FInputActionValue& Value);
	virtual void HandleDodge();
	virtual void HandleInteraction();
	virtual void HandleSkillSlot(const FInputActionValue& Value);

private:
	void BindInputActions();
	void UnbindInputActions();

private:
	UPROPERTY(Transient)
	bool bPlayerComponentInitialized = false;

	uint32 MoveCommandBindingHandle = 0;
	uint32 CursorAimBindingHandle = 0;
	uint32 ZoomBindingHandle = 0;
	uint32 DodgeBindingHandle = 0;
	uint32 InteractionBindingHandle = 0;
	uint32 SkillSlotBindingHandle = 0;
};

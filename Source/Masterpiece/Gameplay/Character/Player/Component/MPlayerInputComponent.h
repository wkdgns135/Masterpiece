// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/SceneComponent.h"
#include "MPlayerInputComponent.generated.h"

class UEnhancedInputComponent;
class UMPlayerInputConfig;
class AMPlayerCharacterBase;

UENUM(BlueprintType)
enum class EMQuickSlot : uint8
{
	Slot1,
	Slot2,
	Slot3,
	Slot4,
	MAX UMETA(Hidden)
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveCommandTriggered, const FInputActionValue&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCursorAimTriggered, const FInputActionValue&);
DECLARE_MULTICAST_DELEGATE(FOnPrimaryAttackTriggered);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnZoomTriggered, float);
DECLARE_MULTICAST_DELEGATE(FOnInteractionTriggered);
DECLARE_MULTICAST_DELEGATE(FOnDodgeTriggered);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillSlotTriggered, int32);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerInputComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMPlayerInputComponent();

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
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UMPlayerInputConfig> InputConfig;
	
private:
	UPROPERTY()
	TObjectPtr<AMPlayerCharacterBase> PlayerCharacter;

	FOnMoveCommandTriggered MoveCommandTriggered;
	FOnCursorAimTriggered CursorAimTriggered;
	FOnPrimaryAttackTriggered PrimaryAttackTriggered;
	FOnZoomTriggered ZoomTriggered;
	FOnInteractionTriggered InteractionTriggered;
	FOnDodgeTriggered DodgeTriggered;
	FOnSkillSlotTriggered SkillSlotTriggered;

public:
	FORCEINLINE FOnMoveCommandTriggered& OnMoveCommandTriggered()
	{
		return MoveCommandTriggered;
	}

	FORCEINLINE FOnCursorAimTriggered& OnCursorAimTriggered()
	{
		return CursorAimTriggered;
	}

	FORCEINLINE FOnPrimaryAttackTriggered& OnPrimaryAttackTriggered()
	{
		return PrimaryAttackTriggered;
	}

	FORCEINLINE FOnZoomTriggered& OnZoomTriggered()
	{
		return ZoomTriggered;
	}

	FORCEINLINE FOnInteractionTriggered& OnInteractionTriggered()
	{
		return InteractionTriggered;
	}

	FORCEINLINE FOnDodgeTriggered& OnDodgeTriggered()
	{
		return DodgeTriggered;
	}

	FORCEINLINE FOnSkillSlotTriggered& OnSkillSlotTriggered()
	{
		return SkillSlotTriggered;
	}
};

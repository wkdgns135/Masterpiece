// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentBase.h"
#include "Components/ActorComponent.h"
#include "MPlayerInputComponent.generated.h"

struct FInputActionValue;
class UInputAction;
class UEnhancedInputComponent;

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
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void ComboAttackPressed(const FInputActionValue& Value);

	UFUNCTION()
	void ChargedAttackStarted(const FInputActionValue& Value);

	UFUNCTION()
	void ChargedAttackReleased(const FInputActionValue& Value);
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MouseLookAction;

	/** Combo Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> ComboAttackAction;

	/** Charged Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> ChargedAttackAction;
};

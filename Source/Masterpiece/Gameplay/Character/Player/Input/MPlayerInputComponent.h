// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerComponentInterface.h"
#include "MPlayerInputComponent.generated.h"

class UMPlayerInputConfig;

UCLASS()
class MASTERPIECE_API UMPlayerInputComponent : public UEnhancedInputComponent, public IMPlayerComponentInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Input")
	const UMPlayerInputConfig* GetInputConfig() const;

	static int32 ResolveSkillSlotIndexFromInput(const FInputActionValue& Value);
};

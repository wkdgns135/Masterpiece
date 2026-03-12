// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerInputComponent.h"

#include "Gameplay/Character/Player/Input/MPlayerInputConfig.h"
#include "Gameplay/Character/Player/Input/MPlayerInputDeveloperSettings.h"

const UMPlayerInputConfig* UMPlayerInputComponent::GetInputConfig() const
{
	const UMPlayerInputDeveloperSettings* InputSettings = GetDefault<UMPlayerInputDeveloperSettings>();
	return InputSettings ? InputSettings->GetDefaultInputConfig() : nullptr;
}

int32 UMPlayerInputComponent::ResolveSkillSlotIndexFromInput(const FInputActionValue& Value)
{
	switch (Value.GetValueType())
	{
	case EInputActionValueType::Boolean:
		return Value.Get<bool>() ? 0 : INDEX_NONE;
	case EInputActionValueType::Axis1D:
		return FMath::RoundToInt(Value.Get<float>());
	case EInputActionValueType::Axis2D:
		return FMath::RoundToInt(Value.Get<FVector2D>().X);
	case EInputActionValueType::Axis3D:
		return FMath::RoundToInt(Value.Get<FVector>().X);
	default:
		return INDEX_NONE;
	}
}

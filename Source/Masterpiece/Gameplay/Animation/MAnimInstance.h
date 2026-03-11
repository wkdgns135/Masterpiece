// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "MAnimInstance.generated.h"

class UMAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MASTERPIECE_API UMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent);
	virtual void NativeInitializeAnimation() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};

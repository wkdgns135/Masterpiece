// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UMAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MASTERPIECE_API UMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	public:
	UMAttributeSet(){}
	
	UMAbilitySystemComponent* GetAbilitySystemComponent() const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTERPIECE_API IMInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void Interact() = 0;
};

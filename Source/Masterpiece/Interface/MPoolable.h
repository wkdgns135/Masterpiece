// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MPoolable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UMPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTERPIECE_API IMPoolable
{
	GENERATED_BODY()

public:
	/** Handles the object being spawned or reused from the pool */
	UFUNCTION(BlueprintCallable, Category="Poolable")
	virtual void Activate() = 0;

	/** Handles the object being returned to the pool */
	UFUNCTION(BlueprintCallable, Category="Poolable")
	virtual void Deactivate() = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MGameplayPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class MASTERPIECE_API AMGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMGameplayPlayerController();

	UFUNCTION(BlueprintCallable, Category="Input")
	bool TraceCursorToWorld(FHitResult& OutHitResult) const;
	
protected:
	virtual void BeginPlay() override;
	
};

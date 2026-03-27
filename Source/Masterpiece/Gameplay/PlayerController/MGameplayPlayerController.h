// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "MGameplayPlayerController.generated.h"

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

	UFUNCTION(BlueprintCallable, Category="UI")
	bool ToggleSkillTreeMenu();
	
protected:
	virtual void BeginPlay() override;

};

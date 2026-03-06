// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/AbilitySystem/MAbilitySystemInterface.h"
#include "MGameplayPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MASTERPIECE_API AMGameplayPlayerState : public APlayerState, public IMAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual UMAbilitySystemComponent* GetAbilitySystemComponent() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<UMAbilitySystemComponent> AbilitySystemComponent;
};

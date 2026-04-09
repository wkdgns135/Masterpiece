// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MPlayerComponentInterface.generated.h"

class AMPlayerCharacterBase;
class AMGameplayPlayerController;
class AMGameplayPlayerState;
class UMPlayerInventoryComponent;
class UMPlayerInputComponent;
UINTERFACE(MinimalAPI, NotBlueprintable)
class UMPlayerComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTERPIECE_API IMPlayerComponentInterface
{
	GENERATED_BODY()

public:
	AMPlayerCharacterBase* GetMPlayerCharacter() const;
	AMGameplayPlayerController* GetMGameplayPlayerController() const;
	AMGameplayPlayerState* GetMGameplayPlayerState() const;
	UMPlayerInventoryComponent* GetMPlayerInventoryComponent() const;
	UMPlayerInputComponent* GetMPlayerInputComponent() const;
};

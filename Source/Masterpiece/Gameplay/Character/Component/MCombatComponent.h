// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MCombatComponent.generated.h"

class AMCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMCombatComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintPure, Category="Combat")
	AMCharacterBase* GetOwnerCharacter() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<AMCharacterBase> OwnerCharacter;
};

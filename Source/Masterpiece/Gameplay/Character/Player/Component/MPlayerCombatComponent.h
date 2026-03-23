// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentInterface.h"
#include "Gameplay/Character/Component/MCombatComponent.h"
#include "MPlayerCombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerCombatComponent : public UMCombatComponent, public IMPlayerComponentInterface
{
	GENERATED_BODY()

public:
	UMPlayerCombatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	void ExecuteDodge();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	void ExecuteInteraction();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	void ExecuteSkillSlot(int32 SkillSlotIndex);
	
	float GetPlayerAttackRange() const;
	float GetPlayerAttackSpeed() const;
};

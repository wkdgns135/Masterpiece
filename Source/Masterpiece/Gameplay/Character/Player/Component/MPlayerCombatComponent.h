// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentInterface.h"
#include "Gameplay/Character/Component/MCombatComponent.h"
#include "MPlayerCombatComponent.generated.h"

class AMPlayerCharacterBase;
struct FGameplayTag;

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
	bool ExecutePrimaryAttack();

	UFUNCTION(BlueprintCallable, Category="Combat|Attack")
	bool CanPrimaryAttack(const AActor* TargetActor) const;
	
	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	void CancelPendingPrimaryAttack();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool ExecuteDodge();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool ExecuteInteraction();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool ExecuteSkillSlot(int32 SkillSlotIndex);
	
	float GetPlayerAttackRange() const;
	
private:
	bool TryActivatePlayerAbilityTag(const FGameplayTag& AbilityTag) const;
	void CancelPlayerAbilityTag(const FGameplayTag& AbilityTag) const;
};

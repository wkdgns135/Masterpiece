// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentInterface.h"
#include "Gameplay/Character/Component/MCombatComponent.h"
#include "MPlayerCombatComponent.generated.h"

struct FInputActionValue;
struct FGameplayEventData;
class AMPlayerCharacterBase;
struct FGameplayTag;
struct FTimerHandle;

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

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool RequestPrimaryAttack(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool ExecuteDodge();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool ExecuteInteraction();

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	bool ExecuteSkillSlot(int32 SkillSlotIndex);
	
private:
	bool IsTargetInAttackRange(const AActor* TargetActor) const;
	float GetPlayerAttackRange() const;
	bool TryActivatePlayerAbilityTag(const FGameplayTag& AbilityTag) const;
	void SendGameplayEventToPlayer(const FGameplayTag& EventTag, const FGameplayEventData& Payload) const;
	void BeginPendingPrimaryAttack(AActor* TargetActor);
	void ClearPendingPrimaryAttack();
	void UpdatePendingPrimaryAttack();

	void BindInputDelegates();
	void UnbindInputDelegates();
	void HandleMoveCommandTriggered(const FInputActionValue& Value);
	void HandleDodgeTriggered();
	void HandleInteractionTriggered();
	void HandleSkillSlotTriggered(int32 SkillSlotIndex);

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> PendingPrimaryAttackTarget;

	FTimerHandle PendingPrimaryAttackTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Attack")
	float AttackRangeTolerance = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Attack")
	float PendingPrimaryAttackUpdateInterval = 0.05f;

	FDelegateHandle MoveCommandDelegateHandle;
	FDelegateHandle DodgeDelegateHandle;
	FDelegateHandle InteractionDelegateHandle;
	FDelegateHandle SkillSlotDelegateHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/Component/MCombatComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"
#include "MPlayerCombatComponent.generated.h"

class AMPlayerCharacterBase;
class UMSkillBase;
struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerCombatComponent : public UMCombatComponent
{
	GENERATED_BODY()

public:
	UMPlayerCombatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category="Combat|Strategy")
	void SetPrimaryAttackStrategy(TSubclassOf<UMSkillBase> StrategyClass);

	UFUNCTION(BlueprintCallable, Category="Combat|Strategy")
	void SetSkillStrategy(EMPlayerSkillType SkillType, TSubclassOf<UMSkillBase> StrategyClass);

	UFUNCTION(BlueprintCallable, Category="Combat|Strategy")
	void ExecutePrimaryAttack();

	UFUNCTION(BlueprintCallable, Category="Combat|Strategy")
	void ExecuteSkill(EMPlayerSkillType SkillType);

	FORCEINLINE AMPlayerCharacterBase* GetPlayerCharacter() const
	{
		return PlayerCharacter;
	}
	
private:
	bool TryActivateAbilityByInputTag(const FGameplayTag& InputTag) const;

	void BindInputDelegates();
	void UnbindInputDelegates();

	void HandlePrimaryAttackTriggered();
	void HandleSkillTriggered(EMPlayerSkillType SkillType);

	UMSkillBase* CreateStrategyInstance(TSubclassOf<UMSkillBase> StrategyClass);
	void InitializeDefaultStrategies();

private:
	UPROPERTY(EditDefaultsOnly, Category="Combat|Strategy")
	TSubclassOf<UMSkillBase> DefaultPrimaryAttackStrategyClass;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Strategy")
	TSubclassOf<UMSkillBase> DefaultSkillQStrategyClass;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Strategy")
	TSubclassOf<UMSkillBase> DefaultSkillWStrategyClass;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Strategy")
	TSubclassOf<UMSkillBase> DefaultSkillEStrategyClass;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Strategy")
	TSubclassOf<UMSkillBase> DefaultSkillRStrategyClass;

	UPROPERTY(Transient)
	TObjectPtr<UMSkillBase> PrimaryAttackStrategy;

	UPROPERTY(Transient)
	TMap<EMPlayerSkillType, TObjectPtr<UMSkillBase>> SkillStrategies;

	UPROPERTY()
	TObjectPtr<AMPlayerCharacterBase> PlayerCharacter;

	FDelegateHandle PrimaryAttackDelegateHandle;
	FDelegateHandle SkillSlotDelegateHandle;
};

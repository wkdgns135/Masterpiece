// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MSkillBase.generated.h"

class UAnimMontage;
class UMCombatComponent;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMSkillBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Combat|Skill")
	bool CanExecute(const UMCombatComponent* CombatComponent) const;
	virtual bool CanExecute_Implementation(const UMCombatComponent* CombatComponent) const;

	UFUNCTION(BlueprintNativeEvent, Category="Combat|Skill")
	void Execute(UMCombatComponent* CombatComponent);
	virtual void Execute_Implementation(UMCombatComponent* CombatComponent);

	UFUNCTION(BlueprintPure, Category="Combat|Skill")
	FName GetSkillId() const;

	UFUNCTION(BlueprintPure, Category="Combat|Skill")
	float GetCooldownSeconds() const;

	UFUNCTION(BlueprintPure, Category="Combat|Skill")
	float GetSkillRange() const;

	UFUNCTION(BlueprintPure, Category="Combat|Skill")
	float GetDamageMultiplier() const;

	UFUNCTION(BlueprintPure, Category="Combat|Skill")
	UAnimMontage* GetSkillMontage() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Skill")
	FName SkillId = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Skill")
	float CooldownSeconds = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Skill")
	float SkillRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Skill")
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Skill")
	TObjectPtr<UAnimMontage> SkillMontage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Types/MStatTypes.h"
#include "Gameplay/Character/MAbilityCharacterBase.h"
#include "Gameplay/Interface/MDamageable.h"
#include "MEnemyCharacterBase.generated.h"

class UAbilitySystemComponent;
class UMAbilitySystemComponent;
class UMCombatAttributeSet;
class UMCombatComponent;

UCLASS(Abstract)
class MASTERPIECE_API AMEnemyCharacterBase : public AMAbilityCharacterBase, public IMDamageable
{
	GENERATED_BODY()

public:
	AMEnemyCharacterBase();
	const FEnemyStat& GetDefaultEnemyStat() const;

	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;

public:
	FORCEINLINE UMCombatComponent* GetCombatComponent() const
	{
		return CombatComponent;
	}

protected:
	virtual UAbilitySystemComponent* ResolveAbilitySystemComponent() const override;
	virtual UMCombatAttributeSet* ResolveCombatAttributeSet() const override;
	virtual const FBaseStat* GetDefaultBaseStat() const override;
	virtual bool AreStartupAbilitiesGranted() const override;
	virtual void SetStartupAbilitiesGranted(bool bGranted) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMCombatAttributeSet> CombatAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMCombatComponent> CombatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes", meta=(AllowPrivateAccess="true"))
	FEnemyStat DefaultEnemyStat;

private:
	UPROPERTY(Transient)
	bool bStartupAbilitiesGranted = false;
};

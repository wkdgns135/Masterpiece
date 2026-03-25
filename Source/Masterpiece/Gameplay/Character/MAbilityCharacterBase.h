// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Core/Types/MStatTypes.h"
#include "Gameplay/Character/MCharacterBase.h"
#include "MAbilityCharacterBase.generated.h"

class UAbilitySystemComponent;
class UMAbilitySystemComponent;
class UMCombatAttributeSet;
class UMGameplayAbility;

UCLASS(Abstract)
class MASTERPIECE_API AMAbilityCharacterBase : public AMCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMAbilityCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UMAbilitySystemComponent* GetMAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMCombatAttributeSet* GetCombatAttributeSet() const;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* ResolveAbilitySystemComponent() const PURE_VIRTUAL(AMAbilityCharacterBase::ResolveAbilitySystemComponent, return nullptr;);
	virtual UMCombatAttributeSet* ResolveCombatAttributeSet() const PURE_VIRTUAL(AMAbilityCharacterBase::ResolveCombatAttributeSet, return nullptr;);
	virtual const FBaseStat* GetDefaultBaseStat() const PURE_VIRTUAL(AMAbilityCharacterBase::GetDefaultBaseStat, return nullptr;);
	virtual bool AreStartupAbilitiesGranted() const PURE_VIRTUAL(AMAbilityCharacterBase::AreStartupAbilitiesGranted, return true;);
	virtual void SetStartupAbilitiesGranted(bool bGranted) PURE_VIRTUAL(AMAbilityCharacterBase::SetStartupAbilitiesGranted, );
	virtual AActor* GetAbilityActorInfoOwner() const;
	virtual AActor* GetAbilityActorInfoAvatar() const;

	virtual void InitializeAbilitySystem();
	virtual void InitializeDefaultAttributes();
	virtual void GrantStartupAbilities();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UMGameplayAbility>> StartupAbilities;
};

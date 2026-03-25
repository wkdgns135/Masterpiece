// Fill out your copyright notice in the Description page of Project Settings.


#include "MEnemyCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AI/MEnemyAIController.h"
#include "Gameplay/AbilitySystem/MAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/Character/Component/MCombatComponent.h"

AMEnemyCharacterBase::AMEnemyCharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CombatAttributeSet = CreateDefaultSubobject<UMCombatAttributeSet>(TEXT("CombatAttributeSet"));
	CombatComponent = CreateDefaultSubobject<UMCombatComponent>(TEXT("CombatComponent"));

	AIControllerClass = AMEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMEnemyCharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
}

void AMEnemyCharacterBase::HandleDeath()
{
}

void AMEnemyCharacterBase::ApplyHealing(float Healing, AActor* Healer)
{
}

const FEnemyStat& AMEnemyCharacterBase::GetDefaultEnemyStat() const
{
	return DefaultEnemyStat;
}

UAbilitySystemComponent* AMEnemyCharacterBase::ResolveAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMCombatAttributeSet* AMEnemyCharacterBase::ResolveCombatAttributeSet() const
{
	return CombatAttributeSet;
}

const FBaseStat* AMEnemyCharacterBase::GetDefaultBaseStat() const
{
	return &DefaultEnemyStat.BaseStat;
}

bool AMEnemyCharacterBase::AreStartupAbilitiesGranted() const
{
	return bStartupAbilitiesGranted;
}

void AMEnemyCharacterBase::SetStartupAbilitiesGranted(const bool bGranted)
{
	bStartupAbilitiesGranted = bGranted;
}

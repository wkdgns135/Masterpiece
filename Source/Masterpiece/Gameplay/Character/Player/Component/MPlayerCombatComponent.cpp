// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/Interface/MDamageable.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"

UMPlayerCombatComponent::UMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMPlayerCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMPlayerCombatComponent::ExecutePrimaryAttack(const AActor* TargetActor)
{
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!PlayerCharacter)
	{
		return;
	}
	FGameplayEventData Payload;
	Payload.EventTag = MGameplayTags::Event_PrimaryAttack_Request;
	Payload.Instigator = PlayerCharacter;
	Payload.Target = TargetActor;
	Payload.EventMagnitude = GetPlayerAttackSpeed();
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, MGameplayTags::Event_PrimaryAttack_Request, Payload);
}

bool UMPlayerCombatComponent::CanPrimaryAttack(const AActor* TargetActor) const
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	if (!TargetActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		return false;
	}
	
	const float DistanceToTarget = FVector::Dist2D(GetMPlayerCharacter()->GetActorLocation(), TargetActor->GetActorLocation());
	const float AttackRange = GetPlayerAttackRange();
	
	return DistanceToTarget <= AttackRange;
}

float UMPlayerCombatComponent::GetPlayerAttackRange() const
{
	const UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	return CombatAttributeSet ? CombatAttributeSet->GetAttackRange() : 0.0f;
}

float UMPlayerCombatComponent::GetPlayerAttackSpeed() const
{
	const UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	return CombatAttributeSet ? CombatAttributeSet->GetAttackSpeed() : 1.0f;
}

const UMCombatAttributeSet* UMPlayerCombatComponent::GetCombatAttributeSet() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	const UMCombatAttributeSet* CombatAttributeSet = PlayerCharacter ? PlayerCharacter->GetCombatAttributeSet() : nullptr;
	return CombatAttributeSet;
}

void UMPlayerCombatComponent::ExecuteDodge()
{
	
}

void UMPlayerCombatComponent::ExecuteInteraction()
{
	
}

void UMPlayerCombatComponent::ExecuteSkillSlot(const int32 SkillSlotIndex)
{
}
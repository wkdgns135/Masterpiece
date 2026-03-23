// Fill out your copyright notice in the Description page of Project Settings.


#include "MCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/Character/MCharacterBase.h"
#include "Gameplay/Interface/MDamageable.h"

UMCombatComponent::UMCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AMCharacterBase>(GetOwner());
}

AMCharacterBase* UMCombatComponent::GetOwnerCharacter() const
{
	return OwnerCharacter;
}

void UMCombatComponent::ExecutePrimaryAttack(const AActor* TargetActor)
{
	AMCharacterBase* OwnerCharacterBase = GetOwnerCharacter();
	if (!OwnerCharacterBase)
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = MGameplayTags::Event_PrimaryAttack_Request;
	Payload.Instigator = OwnerCharacterBase;
	Payload.Target = TargetActor;
	Payload.EventMagnitude = GetAttackSpeed();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerCharacterBase, MGameplayTags::Event_PrimaryAttack_Request, Payload);
}

bool UMCombatComponent::CanPrimaryAttack(const AActor* TargetActor) const
{
	const AMCharacterBase* OwnerCharacterBase = GetOwnerCharacter();
	if (!OwnerCharacterBase || !IsValid(TargetActor))
	{
		return false;
	}

	if (!TargetActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		return false;
	}

	const float DistanceToTarget = FVector::Dist2D(OwnerCharacterBase->GetActorLocation(), TargetActor->GetActorLocation());
	return DistanceToTarget <= GetAttackRange();
}

float UMCombatComponent::GetAttackRange() const
{
	const UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	return CombatAttributeSet ? CombatAttributeSet->GetAttackRange() : 0.0f;
}

float UMCombatComponent::GetAttackSpeed() const
{
	const UMCombatAttributeSet* CombatAttributeSet = GetCombatAttributeSet();
	return CombatAttributeSet ? CombatAttributeSet->GetAttackSpeed() : 1.0f;
}

const UMCombatAttributeSet* UMCombatComponent::GetCombatAttributeSet() const
{
	const AActor* OwnerActor = GetOwner();
	const IAbilitySystemInterface* AbilitySystemOwner = Cast<IAbilitySystemInterface>(OwnerActor);
	if (!AbilitySystemOwner)
	{
		return nullptr;
	}

	const UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemOwner->GetAbilitySystemComponent();
	return AbilitySystemComponent ? AbilitySystemComponent->GetSet<UMCombatAttributeSet>() : nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/Interface/MDamageable.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
#include "TimerManager.h"

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
	ClearPendingPrimaryAttack();
	Super::EndPlay(EndPlayReason);
}


bool UMPlayerCombatComponent::ExecutePrimaryAttack()
{
	return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Attack_Primary);
}

void UMPlayerCombatComponent::CancelPendingPrimaryAttack()
{
	ClearPendingPrimaryAttack();
}

bool UMPlayerCombatComponent::RequestPrimaryAttack(AActor* TargetActor)
{
	if (!IsValid(TargetActor) || !TargetActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		return false;
	}

	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!PlayerCharacter)
	{
		return false;
	}

	if (!IsTargetInAttackRange(TargetActor))
	{
		BeginPendingPrimaryAttack(TargetActor);
		return true;
	}

	ClearPendingPrimaryAttack();
	return ExecutePrimaryAttack();
}

bool UMPlayerCombatComponent::IsTargetInAttackRange(const AActor* TargetActor) const
{
	const float DistanceToTarget = GetDistanceToTarget(TargetActor);
	const float AttackRange = GetPlayerAttackRange();
	return DistanceToTarget <= (AttackRange + AttackRangeTolerance);
}

float UMPlayerCombatComponent::GetPlayerAttackRange() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	const UMCombatAttributeSet* CombatAttributeSet = PlayerCharacter ? PlayerCharacter->GetCombatAttributeSet() : nullptr;
	return CombatAttributeSet ? CombatAttributeSet->GetAttackRange() : 0.0f;
}

bool UMPlayerCombatComponent::ExecuteDodge()
{
	return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Dodge);
}

bool UMPlayerCombatComponent::ExecuteInteraction()
{
	return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Interaction);
}

bool UMPlayerCombatComponent::ExecuteSkillSlot(const int32 SkillSlotIndex)
{
	switch (SkillSlotIndex)
	{
	case 0:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_Q);
	case 1:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_W);
	case 2:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_E);
	case 3:
		return TryActivatePlayerAbilityTag(MGameplayTags::Ability_Skill_R);
	default:
		return false;
	}
}

bool UMPlayerCombatComponent::TryActivatePlayerAbilityTag(const FGameplayTag& AbilityTag) const
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	const AActor* PlayerActor = GetMPlayerCharacter();
	UAbilitySystemComponent* AbilitySystemComponent = PlayerActor
		? UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerActor)
		: nullptr;
	if (!AbilitySystemComponent)
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);
	return AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer, true);
}

void UMPlayerCombatComponent::SendGameplayEventToPlayer(const FGameplayTag& EventTag, const FGameplayEventData& Payload) const
{
	AActor* PlayerActor = GetMPlayerCharacter();
	if (!PlayerActor || !EventTag.IsValid())
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerActor, EventTag, Payload);
}

void UMPlayerCombatComponent::BeginPendingPrimaryAttack(AActor* TargetActor)
{
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	UMPlayerMovementComponent* MovementComponent = PlayerCharacter ? PlayerCharacter->GetPlayerMovementComponent() : nullptr;
	UWorld* World = GetWorld();
	if (!PlayerCharacter || !MovementComponent || !World || !IsValid(TargetActor))
	{
		return;
	}

	PendingPrimaryAttackTarget = TargetActor;
	MovementComponent->DoMoveToTargetActor(TargetActor);

	if (!World->GetTimerManager().IsTimerActive(PendingPrimaryAttackTimerHandle))
	{
		World->GetTimerManager().SetTimer(
			PendingPrimaryAttackTimerHandle,
			this,
			&ThisClass::UpdatePendingPrimaryAttack,
			PendingPrimaryAttackUpdateInterval,
			true);
	}
}

void UMPlayerCombatComponent::ClearPendingPrimaryAttack()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PendingPrimaryAttackTimerHandle);
	}

	PendingPrimaryAttackTarget = nullptr;
}

void UMPlayerCombatComponent::UpdatePendingPrimaryAttack()
{
	AActor* TargetActor = PendingPrimaryAttackTarget.Get();
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	UMPlayerMovementComponent* MovementComponent = PlayerCharacter ? PlayerCharacter->GetPlayerMovementComponent() : nullptr;
	if (!PlayerCharacter || !MovementComponent || !IsValid(TargetActor))
	{
		ClearPendingPrimaryAttack();
		return;
	}

	if (!TargetActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		ClearPendingPrimaryAttack();
		return;
	}

	if (!IsTargetInAttackRange(TargetActor))
	{
		return;
	}

	MovementComponent->StopActiveMovement();
	ClearPendingPrimaryAttack();
	RequestPrimaryAttack(TargetActor);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility_AutoAttack.h"

#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/Component/MPlayerCombatComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

UMPlayerAbility_AutoAttack::UMPlayerAbility_AutoAttack()
{
	AbilityTag = MGameplayTags::Ability_Attack_Auto;
	AbilityTags.AddTag(MGameplayTags::Ability_Attack_Auto);
	bRetriggerInstancedAbility = true;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = MGameplayTags::Event_AutoAttack_Request;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(MGameplayTags::State_AutoAttacking);
	CancelAbilitiesWithTag.AddTag(MGameplayTags::Ability_Movement);
	
	ActivationBlockedTags.AddTag(MGameplayTags::State_Stunned);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Interacting);
	ActivationBlockedTags.AddTag(MGameplayTags::Block_Attack);
}

void UMPlayerAbility_AutoAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentTargetActor = TriggerEventData ? TriggerEventData->Target : nullptr;
	ExecuteAutoAttackStep();
}

void UMPlayerAbility_AutoAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	StopWaitingForGameplayTag(MGameplayTags::State_Attacking);
	if (UMPlayerMovementComponent* MovementComponent = GetMPlayerMovementComponent())
	{
		MovementComponent->OnNavigationMoveFinishedDelegate.RemoveAll(this);
		MovementComponent->StopNavigationMovement();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMPlayerAbility_AutoAttack::ExecuteAutoAttackStep()
{
	UMPlayerCombatComponent* CombatComponent = GetMPlayerCombatComponent();
	UMPlayerMovementComponent* MovementComponent = GetMPlayerMovementComponent();
	if (!CombatComponent || !MovementComponent || !IsValid(CurrentTargetActor))
	{
		EndAbilityAsCancelled();
		return;
	}

	if (CombatComponent->CanPrimaryAttack(CurrentTargetActor))
	{
		MovementComponent->OnNavigationMoveFinishedDelegate.RemoveAll(this);
		MovementComponent->StopNavigationMovement();

		WaitForGameplayTagToBeAddedThenRemoved(MGameplayTags::State_Attacking, this, &ThisClass::HandleAttackFinished);
		CombatComponent->ExecutePrimaryAttack(CurrentTargetActor);
		return;
	}

	// 타겟 액터로 이동
	MovementComponent->OnNavigationMoveFinishedDelegate.RemoveAll(this);
	MovementComponent->OnNavigationMoveFinishedDelegate.AddUObject(this, &ThisClass::HandleMovementFinished);
	MovementComponent->DoMoveToActor(CurrentTargetActor, CombatComponent->GetPlayerAttackRange() - 1.0f);
}

void UMPlayerAbility_AutoAttack::HandleMovementFinished(const bool bReachedTarget)
{
	if (!bReachedTarget)
	{
		EndAbilityAsCancelled();
		return;
	}

	ExecuteAutoAttackStep();
}

void UMPlayerAbility_AutoAttack::HandleAttackFinished()
{
	// TODO : 공격 루프 수정 근데
	auto* Task = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, MGameplayTags::State_AttackReady);
	Task->Added.AddDynamic(this, &ThisClass::ExecuteAutoAttackStep);
	Task->ReadyForActivation();
}

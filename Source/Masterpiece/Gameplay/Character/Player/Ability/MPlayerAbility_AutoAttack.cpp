// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility_AutoAttack.h"

#include "AbilitySystemComponent.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/Component/MPlayerCombatComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
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

	const AActor* RequestedTargetActor = TriggerEventData ? TriggerEventData->Target.Get() : nullptr;
	if (RequestedTargetActor == CurrentTargetActor.Get() && IsValid(RequestedTargetActor))
	{
		const UAbilitySystemComponent* AbilitySystemComponent = GetOwnerAbilitySystemComponent();
		if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(MGameplayTags::State_Attacking))
		{
			EndAbilityAsSuccess();
			return;
		}
	}

	if (IsValid(RequestedTargetActor))
	{
		CurrentTargetActor = RequestedTargetActor;
	}

	ExecuteAutoAttackStep();
}

void UMPlayerAbility_AutoAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
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
		
		CombatComponent->ExecutePrimaryAttack(CurrentTargetActor);
		EndAbilityAsSuccess();
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

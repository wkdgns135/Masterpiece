// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility_AutoAttack.h"

#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/Component/MPlayerCombatComponent.h"

UMPlayerAbility_AutoAttack::UMPlayerAbility_AutoAttack()
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = MGameplayTags::Event_Attack_Request;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.AddTag(MGameplayTags::State_Stunned);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Interacting);
	ActivationBlockedTags.AddTag(MGameplayTags::Block_Attack);
}

void UMPlayerAbility_AutoAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UMPlayerCombatComponent* CombatComponent = GetMPlayerCombatComponent();
	AActor* TargetActor = TriggerEventData ? const_cast<AActor*>(TriggerEventData->Target.Get()) : nullptr;
	const bool bSucceeded = CombatComponent && IsValid(TargetActor) && CombatComponent->RequestPrimaryAttack(TargetActor);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, !bSucceeded);
}

void UMPlayerAbility_AutoAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

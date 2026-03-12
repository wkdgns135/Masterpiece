// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility_Movement.h"

#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"

UMPlayerAbility_Movement::UMPlayerAbility_Movement()
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = MGameplayTags::Event_Move_Request;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.AddTag(MGameplayTags::State_Interacting);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Stunned);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(MGameplayTags::Block_Movement);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Attacking);
}

void UMPlayerAbility_Movement::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UMPlayerMovementComponent* MovementComponent = GetMPlayerMovementComponent();
	if (!MovementComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AActor* TargetActor = TriggerEventData ? TriggerEventData->Target : nullptr;
	if (IsValid(TargetActor))
	{
		MovementComponent->DoMoveToTargetActor(const_cast<AActor*>(TargetActor));
	}
	else
	{
		MovementComponent->DoMoveToCursor();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UMPlayerAbility_Movement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility_Movement.h"

#include "Gameplay/MGameplayTags.h"
#include "Gameplay/AbilitySystem/Attribute/MCombatAttributeSet.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
#include "Gameplay/Interface/MDamageable.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"

UMPlayerAbility_Movement::UMPlayerAbility_Movement()
{
	AbilityTag = MGameplayTags::Ability_Movement;
	AbilityTags.AddTag(MGameplayTags::Ability_Movement);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = MGameplayTags::Event_Move_Request;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(MGameplayTags::State_Moving);
	CancelAbilitiesWithTag.AddTag(MGameplayTags::Ability_Attack_Auto);

	ActivationBlockedTags.AddTag(MGameplayTags::State_Attacking);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Interacting);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Stunned);
	ActivationBlockedTags.AddTag(MGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(MGameplayTags::Block_Movement);

	bRetriggerInstancedAbility = true;
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

	MovementComponent->OnNavigationMoveFinishedDelegate.RemoveAll(this);
	MovementComponent->OnNavigationMoveFinishedDelegate.AddUObject(this, &ThisClass::HandleNavigationMoveFinished);

	auto CancelMovementAbility = [this, MovementComponent, Handle, ActorInfo, ActivationInfo]()
	{
		MovementComponent->OnNavigationMoveFinishedDelegate.RemoveAll(this);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	};

	const AActor* TargetActor = TriggerEventData ? TriggerEventData->Target : nullptr;
	if (IsValid(TargetActor))
	{
		const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
		const UMCombatAttributeSet* CombatAttributeSet = PlayerCharacter ? PlayerCharacter->GetCombatAttributeSet() : nullptr;
		const float AcceptanceRadius = CombatAttributeSet ? CombatAttributeSet->GetAttackRange() : 0.0f;
		MovementComponent->DoMoveToActor(TargetActor, AcceptanceRadius);
	}
	else
	{
		FVector Destination = FVector::ZeroVector;
		if (ResolveMoveDestination(TriggerEventData, Destination))
		{
			MovementComponent->DoMoveToLocation(Destination);
		}
	}
}

void UMPlayerAbility_Movement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UMPlayerMovementComponent* MovementComponent = GetMPlayerMovementComponent())
	{
		MovementComponent->OnNavigationMoveFinishedDelegate.RemoveAll(this);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMPlayerAbility_Movement::HandleNavigationMoveFinished(const bool bReachedTarget)
{
	if (bReachedTarget)
	{
		EndAbilityAsSuccess();
		return;
	}

	EndAbilityAsCancelled();
}

bool UMPlayerAbility_Movement::ResolveMoveDestination(const FGameplayEventData* TriggerEventData, FVector& OutDestination) const
{
	auto IsInvalidMoveHitActor = [](const AActor* HitActor)
	{
		return IsValid(HitActor) && HitActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass());
	};

	if (const FHitResult* HitResult = TriggerEventData ? TriggerEventData->ContextHandle.GetHitResult() : nullptr)
	{
		if (!IsInvalidMoveHitActor(HitResult->GetActor()))
		{
			OutDestination = HitResult->ImpactPoint;
			return true;
		}
	}

	const AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	FHitResult CursorHit;
	if (!PlayerController->TraceCursorToWorld(CursorHit))
	{
		return false;
	}

	if (IsInvalidMoveHitActor(CursorHit.GetActor()))
	{
		return false;
	}

	OutDestination = CursorHit.ImpactPoint;
	return true;
}

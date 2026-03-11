// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

#include "Gameplay/AbilitySystem/MAbilitySystemInterface.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"

namespace
{
bool TryActivatePlayerAbilityTag(AMPlayerCharacterBase* PlayerCharacter, const FGameplayTag& AbilityTag)
{
	return PlayerCharacter ? PlayerCharacter->TryActivateAbilityByTag(AbilityTag) : false;
}
}

UMPlayerCombatComponent::UMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	BindInputDelegates();
}

void UMPlayerCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindInputDelegates();
	Super::EndPlay(EndPlayReason);
}


bool UMPlayerCombatComponent::ExecutePrimaryAttack()
{
	return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Attack_Primary);
}

bool UMPlayerCombatComponent::ExecuteDodge()
{
	return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Dodge);
}

bool UMPlayerCombatComponent::ExecuteInteraction()
{
	return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Interaction);
}

bool UMPlayerCombatComponent::ExecuteSkillSlot(const int32 SkillSlotIndex)
{
	switch (SkillSlotIndex)
	{
	case 0:
		return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Skill_Q);
	case 1:
		return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Skill_W);
	case 2:
		return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Skill_E);
	case 3:
		return TryActivatePlayerAbilityTag(GetPlayerCharacter(), MGameplayTags::Ability_Skill_R);
	default:
		return false;
	}
}

void UMPlayerCombatComponent::BindInputDelegates()
{
	const AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return;
	}

	UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent();
	check(InputComponent);

	PrimaryAttackDelegateHandle = InputComponent->OnPrimaryAttackTriggered().AddUObject(this, &ThisClass::HandlePrimaryAttackTriggered);
	DodgeDelegateHandle = InputComponent->OnDodgeTriggered().AddUObject(this, &ThisClass::HandleDodgeTriggered);
	InteractionDelegateHandle = InputComponent->OnInteractionTriggered().AddUObject(this, &ThisClass::HandleInteractionTriggered);
	SkillSlotDelegateHandle = InputComponent->OnSkillSlotTriggered().AddUObject(this, &ThisClass::HandleSkillSlotTriggered);
}

void UMPlayerCombatComponent::UnbindInputDelegates()
{
	const AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return;
	}

	UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent();
	check(InputComponent);

	InputComponent->OnPrimaryAttackTriggered().Remove(PrimaryAttackDelegateHandle);
	InputComponent->OnDodgeTriggered().Remove(DodgeDelegateHandle);
	InputComponent->OnInteractionTriggered().Remove(InteractionDelegateHandle);
	InputComponent->OnSkillSlotTriggered().Remove(SkillSlotDelegateHandle);
}

void UMPlayerCombatComponent::HandlePrimaryAttackTriggered()
{
	ExecutePrimaryAttack();
}

void UMPlayerCombatComponent::HandleDodgeTriggered()
{
	ExecuteDodge();
}

void UMPlayerCombatComponent::HandleInteractionTriggered()
{
	ExecuteInteraction();
}

void UMPlayerCombatComponent::HandleSkillSlotTriggered(const int32 SkillSlotIndex)
{
	ExecuteSkillSlot(SkillSlotIndex);
}

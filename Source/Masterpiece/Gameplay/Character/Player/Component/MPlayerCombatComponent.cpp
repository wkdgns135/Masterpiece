// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Gameplay/AbilitySystem/MGameplayTags.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"

UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_Q, "Input.Ability_Skill_Q");
UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_W, "Input.Ability_Skill_W");
UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_E, "Input.Ability_Skill_E");
UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_R, "Input.Ability_Skill_R");

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
	return TryActivateAbilityByInputTag(MGameplayTags::Input_Ability_PrimaryAttack);
}

bool UMPlayerCombatComponent::ExecuteDodge()
{
	return TryActivateAbilityByInputTag(MGameplayTags::Input_Ability_Dodge);
}

bool UMPlayerCombatComponent::ExecuteInteraction()
{
	return TryActivateAbilityByInputTag(MGameplayTags::Input_Action_Interaction);
}

bool UMPlayerCombatComponent::ExecuteSkillSlot(const int32 SkillSlotIndex)
{
	switch (SkillSlotIndex)
	{
	case 0:
		return TryActivateAbilityByInputTag(Input_Ability_Skill_Q);
	case 1:
		return TryActivateAbilityByInputTag(Input_Ability_Skill_W);
	case 2:
		return TryActivateAbilityByInputTag(Input_Ability_Skill_E);
	case 3:
		return TryActivateAbilityByInputTag(Input_Ability_Skill_R);
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
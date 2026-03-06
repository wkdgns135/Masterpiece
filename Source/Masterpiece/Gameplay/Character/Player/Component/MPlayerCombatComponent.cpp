// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Gameplay/AbilitySystem/MGameplayTags.h"
#include "Gameplay/Character/Skill/MSkillBase.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerInputComponent.h"

UMPlayerCombatComponent::UMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwner());
	check(PlayerCharacter);

	InitializeDefaultStrategies();
	BindInputDelegates();
}

void UMPlayerCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindInputDelegates();
	Super::EndPlay(EndPlayReason);
}

void UMPlayerCombatComponent::SetPrimaryAttackStrategy(const TSubclassOf<UMSkillBase> StrategyClass)
{
	PrimaryAttackStrategy = CreateStrategyInstance(StrategyClass);
}

void UMPlayerCombatComponent::SetSkillStrategy(const EMPlayerSkillType SkillType, const TSubclassOf<UMSkillBase> StrategyClass)
{
	if (SkillType == EMPlayerSkillType::MAX)
	{
		return;
	}

	SkillStrategies.FindOrAdd(SkillType) = CreateStrategyInstance(StrategyClass);
}

void UMPlayerCombatComponent::ExecutePrimaryAttack()
{
	if (TryActivateAbilityByInputTag(MGameplayTags::Input_Ability_PrimaryAttack))
	{
		return;
	}

	if (!IsValid(PrimaryAttackStrategy))
	{
		return;
	}

	if (!PrimaryAttackStrategy->CanExecute(this))
	{
		return;
	}

	PrimaryAttackStrategy->Execute(this);
	CommitSkillUsage(PrimaryAttackStrategy->GetSkillId());
}

void UMPlayerCombatComponent::ExecuteSkill(const EMPlayerSkillType SkillType)
{
	if (SkillType == EMPlayerSkillType::MAX)
	{
		return;
	}

	TObjectPtr<UMSkillBase>* FoundStrategyPtr = SkillStrategies.Find(SkillType);
	if (!FoundStrategyPtr || !IsValid(*FoundStrategyPtr))
	{
		return;
	}

	if (!(*FoundStrategyPtr)->CanExecute(this))
	{
		return;
	}

	(*FoundStrategyPtr)->Execute(this);
	CommitSkillUsage((*FoundStrategyPtr)->GetSkillId());
}

void UMPlayerCombatComponent::BindInputDelegates()
{
	if (!PlayerCharacter)
	{
		return;
	}

	UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent();
	check(InputComponent);

	PrimaryAttackDelegateHandle = InputComponent->OnPrimaryAttackTriggered().AddUObject(this, &ThisClass::HandlePrimaryAttackTriggered);
	SkillSlotDelegateHandle = InputComponent->OnSkillSlotTriggered().AddUObject(this, &ThisClass::HandleSkillTriggered);
}

void UMPlayerCombatComponent::UnbindInputDelegates()
{
	if (!PlayerCharacter)
	{
		return;
	}

	UMPlayerInputComponent* InputComponent = PlayerCharacter->GetPlayerInputComponent();
	check(InputComponent);

	InputComponent->OnPrimaryAttackTriggered().Remove(PrimaryAttackDelegateHandle);
	InputComponent->OnSkillSlotTriggered().Remove(SkillSlotDelegateHandle);
}

void UMPlayerCombatComponent::HandlePrimaryAttackTriggered()
{
	ExecutePrimaryAttack();
}

void UMPlayerCombatComponent::HandleSkillTriggered(const EMPlayerSkillType SkillType)
{
	ExecuteSkill(SkillType);
}

UMSkillBase* UMPlayerCombatComponent::CreateStrategyInstance(
	const TSubclassOf<UMSkillBase> StrategyClass)
{
	if (!StrategyClass)
	{
		return nullptr;
	}

	return NewObject<UMSkillBase>(this, StrategyClass);
}

void UMPlayerCombatComponent::InitializeDefaultStrategies()
{
	SetPrimaryAttackStrategy(DefaultPrimaryAttackStrategyClass);
	SetSkillStrategy(EMPlayerSkillType::SkillQ, DefaultSkillQStrategyClass);
	SetSkillStrategy(EMPlayerSkillType::SkillW, DefaultSkillWStrategyClass);
	SetSkillStrategy(EMPlayerSkillType::SkillE, DefaultSkillEStrategyClass);
	SetSkillStrategy(EMPlayerSkillType::SkillR, DefaultSkillRStrategyClass);
}

bool UMPlayerCombatComponent::TryActivateAbilityByInputTag(const FGameplayTag& InputTag) const
{
	if (!InputTag.IsValid() || !PlayerCharacter)
	{
		return false;
	}

	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(PlayerCharacter);
	if (!AbilitySystemInterface)
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(InputTag);
	return AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer, true);
}

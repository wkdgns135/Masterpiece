// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerComponentInterface.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"


// Add default functionality here for any IMPlayerComponentInterface functions that are not pure virtual.
AMPlayerCharacterBase* IMPlayerComponentInterface::GetPlayerCharacter() const
{
	if (const UActorComponent* Component = Cast<UActorComponent>(this))
	{
		return Cast<AMPlayerCharacterBase>(Component->GetOwner());
	}
	return nullptr;
}

bool IMPlayerComponentInterface::TryActivateAbilityByInputTag(const FGameplayTag& InputTag) const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
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

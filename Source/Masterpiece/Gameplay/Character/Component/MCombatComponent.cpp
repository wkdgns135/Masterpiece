// Fill out your copyright notice in the Description page of Project Settings.


#include "MCombatComponent.h"

#include "Gameplay/Character/MCharacterBase.h"

UMCombatComponent::UMCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AMCharacterBase>(GetOwner());
}

AMCharacterBase* UMCombatComponent::GetOwnerCharacter() const
{
	return OwnerCharacter;
}

bool UMCombatComponent::IsSkillOnCooldown(const FName SkillId, const float CooldownSeconds) const
{
	if (SkillId.IsNone() || CooldownSeconds <= 0.0f)
	{
		return false;
	}

	const float* LastUseTime = LastSkillUseTime.Find(SkillId);
	if (!LastUseTime)
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	return (World->GetTimeSeconds() - *LastUseTime) < CooldownSeconds;
}

void UMCombatComponent::CommitSkillUsage(const FName SkillId)
{
	if (SkillId.IsNone())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		LastSkillUseTime.Add(SkillId, World->GetTimeSeconds());
	}
}


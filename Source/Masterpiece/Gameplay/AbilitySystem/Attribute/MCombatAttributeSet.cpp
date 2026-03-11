#include "MCombatAttributeSet.h"

#include "Net/UnrealNetwork.h"

UMCombatAttributeSet::UMCombatAttributeSet()
{
}

void UMCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, AttackRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatAttributeSet, Defense, COND_None, REPNOTIFY_Always);
}

void UMCombatAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, Health, OldValue);
}

void UMCombatAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, MaxHealth, OldValue);
}

void UMCombatAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, MoveSpeed, OldValue);
}

void UMCombatAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, AttackPower, OldValue);
}

void UMCombatAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, AttackSpeed, OldValue);
}

void UMCombatAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, AttackRange, OldValue);
}

void UMCombatAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCombatAttributeSet, Defense, OldValue);
}

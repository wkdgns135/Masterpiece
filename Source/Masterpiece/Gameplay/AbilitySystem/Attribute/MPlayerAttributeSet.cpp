#include "MPlayerAttributeSet.h"

#include "Net/UnrealNetwork.h"

UMPlayerAttributeSet::UMPlayerAttributeSet()
{
}

void UMPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMPlayerAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMPlayerAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UMPlayerAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMPlayerAttributeSet, Mana, OldValue);
}

void UMPlayerAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMPlayerAttributeSet, MaxMana, OldValue);
}

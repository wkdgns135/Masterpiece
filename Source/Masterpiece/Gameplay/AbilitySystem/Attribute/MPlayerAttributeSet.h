#pragma once

#include "CoreMinimal.h"
#include "MAttributeSet.h"
#include "MPlayerAttributeSet.generated.h"

UCLASS()
class MASTERPIECE_API UMPlayerAttributeSet : public UMAttributeSet
{
	GENERATED_BODY()

public:
	UMPlayerAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Player", ReplicatedUsing=OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMPlayerAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Player", ReplicatedUsing=OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMPlayerAttributeSet, MaxMana)

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;
};

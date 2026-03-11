#pragma once

#include "CoreMinimal.h"
#include "MAttributeSet.h"
#include "MCombatAttributeSet.generated.h"

UCLASS()
class MASTERPIECE_API UMCombatAttributeSet : public UMAttributeSet
{
	GENERATED_BODY()

public:
	UMCombatAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_AttackRange)
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, AttackRange)

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Combat", ReplicatedUsing=OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UMCombatAttributeSet, Defense)

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_AttackRange(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue) const;
};

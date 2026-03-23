// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MCombatComponent.generated.h"

class AMCharacterBase;
class UMCombatAttributeSet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMCombatComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintPure, Category="Combat")
	AMCharacterBase* GetOwnerCharacter() const;

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	virtual void ExecutePrimaryAttack(const AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Combat|Attack")
	virtual bool CanPrimaryAttack(const AActor* TargetActor) const;

	UFUNCTION(BlueprintPure, Category="Combat|Attack")
	virtual float GetAttackRange() const;

	UFUNCTION(BlueprintPure, Category="Combat|Attack")
	virtual float GetAttackSpeed() const;

protected:
	virtual const UMCombatAttributeSet* GetCombatAttributeSet() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<AMCharacterBase> OwnerCharacter;
};

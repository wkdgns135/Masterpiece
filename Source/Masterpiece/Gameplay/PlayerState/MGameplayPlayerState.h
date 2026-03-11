// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MGameplayPlayerState.generated.h"

class UMAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MASTERPIECE_API AMGameplayPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMGameplayPlayerState();
	
	UMAbilitySystemComponent* GetMAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UMCombatAttributeSet* GetCombatAttributeSet() const;
	class UMPlayerAttributeSet* GetPlayerAttributeSet() const;

	bool AreStartupAbilitiesGranted() const;
	void SetStartupAbilitiesGranted(bool bGranted);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<UMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<class UMCombatAttributeSet> CombatAttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<class UMPlayerAttributeSet> PlayerAttributeSet;

	UPROPERTY(Transient)
	bool bStartupAbilitiesGranted = false;
};

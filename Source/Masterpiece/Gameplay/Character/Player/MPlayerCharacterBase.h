// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/AbilitySystem/MAbilitySystemInterface.h"
#include "Core/Types/MStatTypes.h"
#include "Gameplay/Character/MCharacterBase.h"
#include "Gameplay/Character/Interface/MAttacker.h"
#include "Gameplay/Interface/MDamageable.h"
#include "MPlayerCharacterBase.generated.h"

class UAbilitySystemComponent;
class UMAbilitySystemComponent;
class UMAttributeSet;
class UMGameplayAbility;
class UMPlayerCameraComponent;
class UMPlayerCombatComponent;
class UMPlayerInputComponent;
class UMPlayerMovementComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract)
class MASTERPIECE_API AMPlayerCharacterBase : public AMCharacterBase, public IMAbilitySystemInterface, public IMDamageable, public IMAttacker
{
	GENERATED_BODY()

public:
	AMPlayerCharacterBase();
	virtual UMAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;
	
	// MDamageable interface
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;
	
	// MAttacker interface
	virtual void DoAttackTrace(FName DamageSourceBone) override;
	
protected:
	/** 플레이어 전용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerInputComponent> PlayerInputComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerMovementComponent> PlayerMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerCameraComponent> PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerCombatComponent> PlayerCombatComponent;

	/** 공용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UMGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes", meta = (AllowPrivateAccess = "true"))
	FPlayerStat DefaultPlayerStat;

private:
	void InitializeAbilitySystem();
	void InitializeDefaultAttributes();
	void GrantStartupAbilities();
	class AMGameplayPlayerState* GetGameplayPlayerState() const;
	
public:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const
	{
		check(SpringArmComponent);
		return SpringArmComponent;
	}
	FORCEINLINE UCameraComponent* GetFollowCameraComponent() const
	{
		check(FollowCameraComponent);
		return FollowCameraComponent;
	}
	FORCEINLINE UMPlayerMovementComponent* GetPlayerMovementComponent() const
	{
		check(PlayerMovementComponent);
		return PlayerMovementComponent;
	}
	FORCEINLINE UMPlayerCameraComponent* GetPlayerCameraComponent() const
	{
		check(PlayerCameraComponent);
		return PlayerCameraComponent;
	}
	FORCEINLINE UMPlayerInputComponent* GetPlayerInputComponent() const
	{
		check(PlayerInputComponent);
		return PlayerInputComponent;
	}
	FORCEINLINE UMPlayerCombatComponent* GetPlayerCombatComponent() const
	{
		check(PlayerCombatComponent);
		return PlayerCombatComponent;
	}
	UMAttributeSet* GetAttributeSet() const;
};

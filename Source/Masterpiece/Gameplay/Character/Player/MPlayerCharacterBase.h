// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Types/MStatTypes.h"
#include "Gameplay/Character/MAbilityCharacterBase.h"
#include "Gameplay/Character/Interface/MAttacker.h"
#include "Gameplay/Interface/MDamageable.h"
#include "MPlayerCharacterBase.generated.h"

class UAbilitySystemComponent;
class UMCombatAttributeSet;
class UMPlayerCameraComponent;
class UMPlayerComponent;
class UMPlayerSkillComponent;
class UMPlayerAttributeSet;
class UMPlayerCombatComponent;
class UMPlayerInputComponent;
class UMPlayerMovementComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract)
class MASTERPIECE_API AMPlayerCharacterBase : public AMAbilityCharacterBase, public IMDamageable, public IMAttacker
{
	GENERATED_BODY()

public:
	AMPlayerCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// MDamageable interface
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;
	
	// MAttacker interface
	virtual void DoAttackTrace(FName DamageSourceBone) override;
	
protected:
	/** 플레이어 전용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerComponent> PlayerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerCameraComponent> PlayerCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerCombatComponent> PlayerCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMPlayerSkillComponent> SkillComponent;
	
	/** 공용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes", meta = (AllowPrivateAccess = "true"))
	FPlayerStat DefaultPlayerStat;

private:
	class AMGameplayPlayerState* GetGameplayPlayerState() const;

protected:
	virtual UAbilitySystemComponent* ResolveAbilitySystemComponent() const override;
	virtual UMCombatAttributeSet* ResolveCombatAttributeSet() const override;
	virtual const FBaseStat* GetDefaultBaseStat() const override;
	virtual bool AreStartupAbilitiesGranted() const override;
	virtual void SetStartupAbilitiesGranted(bool bGranted) override;
	virtual AActor* GetAbilityActorInfoOwner() const override;
	virtual void InitializeDefaultAttributes() override;
	
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
	FORCEINLINE UMPlayerCameraComponent* GetPlayerCameraComponent() const
	{
		check(PlayerCameraComponent);
		return PlayerCameraComponent;
	}
	FORCEINLINE UMPlayerComponent* GetPlayerComponent() const
	{
		check(PlayerComponent);
		return PlayerComponent;
	}
	FORCEINLINE UMPlayerCombatComponent* GetPlayerCombatComponent() const
	{
		check(PlayerCombatComponent);
		return PlayerCombatComponent;
	}
	FORCEINLINE UMPlayerSkillComponent* GetSkillComponent() const
	{
		check(SkillComponent);
		return SkillComponent;
	}
	UMPlayerInputComponent* GetPlayerInputComponent() const;
	UMPlayerMovementComponent* GetPlayerMovementComponent() const;
	UMPlayerAttributeSet* GetPlayerAttributeSet() const;
};

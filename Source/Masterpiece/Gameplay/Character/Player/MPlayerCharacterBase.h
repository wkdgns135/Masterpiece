// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/MCharacterBase.h"
#include "Gameplay/Character/Interface/MAttacker.h"
#include "Gameplay/Interface/MDamageable.h"
#include "MPlayerCharacterBase.generated.h"

class UMPlayerCameraComponent;
class UMPlayerInputComponent;
class UMPlayerMovementComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MASTERPIECE_API AMPlayerCharacterBase : public AMCharacterBase, public IMDamageable, public IMAttacker
{
	GENERATED_BODY()

public:
	AMPlayerCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// MDamageable interface
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;
	
	// MAttacker interface
	virtual void DoAttackTrace(FName DamageSourceBone) override;
	virtual void CheckChargedAttack() override;
	virtual void CheckCombo() override;
	
protected:
	/** 플레이어 전용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerInputComponent> PlayerInput;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerMovementComponent> PlayerMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMPlayerCameraComponent> PlayerCamera;

	/** 공용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const
	{
		check(SpringArm);
		return SpringArm;
	}
	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		check(FollowCamera);
		return FollowCamera;
	}
	FORCEINLINE UMPlayerMovementComponent* GetPlayerMovement() const
	{
		check(PlayerMovement);
		return PlayerMovement;
	}
};

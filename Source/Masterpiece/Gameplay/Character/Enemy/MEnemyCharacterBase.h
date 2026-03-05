// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/MCharacterBase.h"
#include "Gameplay/Interface/MDamageable.h"
#include "MEnemyCharacterBase.generated.h"

UCLASS(Abstract)
class MASTERPIECE_API AMEnemyCharacterBase : public AMCharacterBase, public IMDamageable
{
	GENERATED_BODY()

public:
	AMEnemyCharacterBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;
};

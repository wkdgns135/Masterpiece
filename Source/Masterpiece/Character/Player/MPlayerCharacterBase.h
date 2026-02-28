// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MCharacterBase.h"
#include "Interface/MDamageable.h"
#include "MPlayerCharacterBase.generated.h"

UCLASS()
class MASTERPIECE_API AMPlayerCharacterBase : public AMCharacterBase, public IMDamageable
{
	GENERATED_BODY()

public:
	AMPlayerCharacterBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;
};

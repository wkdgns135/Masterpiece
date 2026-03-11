// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Interface/MDamageable.h"
#include "MDamageableDummy.generated.h"

UCLASS()
class MASTERPIECE_API AMDamageableDummy : public AActor, public IMDamageable, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AMDamageableDummy();
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
protected:
	virtual void BeginPlay() override;
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;
	virtual void HandleDeath() override;
	virtual void ApplyHealing(float Healing, AActor* Healer) override;
	
};

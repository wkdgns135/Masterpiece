// Fill out your copyright notice in the Description page of Project Settings.


#include "MEnemyCharacterBase.h"

#include "Gameplay/Character/Component/MCombatComponent.h"

AMEnemyCharacterBase::AMEnemyCharacterBase()
{
	CombatComponent = CreateDefaultSubobject<UMCombatComponent>(TEXT("CombatComponent"));
}

void AMEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMEnemyCharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
}

void AMEnemyCharacterBase::HandleDeath()
{
}

void AMEnemyCharacterBase::ApplyHealing(float Healing, AActor* Healer)
{
}

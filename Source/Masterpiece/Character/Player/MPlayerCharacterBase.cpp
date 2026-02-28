// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacterBase.h"


AMPlayerCharacterBase::AMPlayerCharacterBase()
{
}

void AMPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMPlayerCharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
}

void AMPlayerCharacterBase::HandleDeath()
{
}

void AMPlayerCharacterBase::ApplyHealing(float Healing, AActor* Healer)
{
}

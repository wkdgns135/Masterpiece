// Fill out your copyright notice in the Description page of Project Settings.


#include "MDamageableDummy.h"

#include "Gameplay/MGameplayTags.h"


// Sets default values
AMDamageableDummy::AMDamageableDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AMDamageableDummy::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	
}

// Called when the game starts or when spawned
void AMDamageableDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMDamageableDummy::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
}

void AMDamageableDummy::HandleDeath()
{
}

void AMDamageableDummy::ApplyHealing(float Healing, AActor* Healer)
{
}



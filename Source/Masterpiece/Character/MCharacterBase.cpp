// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacterBase.h"


AMCharacterBase::AMCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacterBase.h"


AMCharacterBase::AMCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

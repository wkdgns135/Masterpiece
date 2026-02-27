// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerComponentBase.h"

#include "Character/Player/MPlayerCharacterBase.h"


UMPlayerComponentBase::UMPlayerComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UMPlayerComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwner());
	check(PlayerCharacter);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerComponentInterface.h"

#include "Gameplay/Character/Player/MPlayerCharacterBase.h"

AMPlayerCharacterBase* IMPlayerComponentInterface::GetPlayerCharacter() const
{
	if (const UActorComponent* Component = Cast<UActorComponent>(this))
	{
		return Cast<AMPlayerCharacterBase>(Component->GetOwner());
	}
	return nullptr;
}

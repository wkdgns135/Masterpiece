// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerAbility.h"

#include "Gameplay/Character/Player/MPlayerCharacterBase.h"

AMPlayerCharacterBase* UMPlayerAbility::GetPlayerCharacter() const
{
	return Cast<AMPlayerCharacterBase>(GetAvatarActorFromActorInfo());
}

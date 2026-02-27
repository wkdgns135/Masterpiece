// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MCharacterBase.h"
#include "MNpcCharacterBase.generated.h"

UCLASS()
class MASTERPIECE_API AMNpcCharacterBase : public AMCharacterBase
{
	GENERATED_BODY()

public:
	AMNpcCharacterBase();

protected:
	virtual void BeginPlay() override;

};

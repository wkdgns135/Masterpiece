// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacterBase.generated.h"

UCLASS()
class MASTERPIECE_API AMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMCharacterBase();

protected:
	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/MCharacterBase.h"
#include "Gameplay/Interface/MInteractable.h"
#include "MNpcCharacterBase.generated.h"

UCLASS(Abstract)
class MASTERPIECE_API AMNpcCharacterBase : public AMCharacterBase, public IMInteractable
{
	GENERATED_BODY()

public:
	AMNpcCharacterBase();

protected:
	virtual void BeginPlay() override;

	virtual void Interaction(AActor* Interactor) override;
};

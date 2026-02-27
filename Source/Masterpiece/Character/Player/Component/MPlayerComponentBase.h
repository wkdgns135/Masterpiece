// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MPlayerComponentBase.generated.h"


class AMPlayerCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UMPlayerComponentBase();

protected:
	virtual void BeginPlay() override;
	FORCEINLINE AMPlayerCharacterBase* GetPlayerCharacter(){return PlayerCharacter;};
	
private:
	UPROPERTY()
	TObjectPtr<AMPlayerCharacterBase> PlayerCharacter;
};

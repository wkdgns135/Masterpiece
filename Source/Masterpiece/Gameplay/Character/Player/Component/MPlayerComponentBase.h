// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MPlayerComponentBase.generated.h"


class AMPlayerCharacterBase;

UCLASS(ClassGroup=(Custom), NotBlueprintable)
class MASTERPIECE_API UMPlayerComponentBase : public USceneComponent
{
	GENERATED_BODY()

public:
	UMPlayerComponentBase();
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<AMPlayerCharacterBase> CachedPlayerCharacter;
	
public:
	UFUNCTION(BlueprintCallable, Category="Player")
	FORCEINLINE AMPlayerCharacterBase* GetPlayerCharacter()
	{
		check(CachedPlayerCharacter);
		return CachedPlayerCharacter;
	}
};

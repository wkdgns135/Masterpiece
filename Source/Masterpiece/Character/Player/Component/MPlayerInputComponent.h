// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentBase.h"
#include "Components/ActorComponent.h"
#include "MPlayerInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerInputComponent : public UMPlayerComponentBase
{
	GENERATED_BODY()

public:
	UMPlayerInputComponent();

protected:
	virtual void BeginPlay() override;

};

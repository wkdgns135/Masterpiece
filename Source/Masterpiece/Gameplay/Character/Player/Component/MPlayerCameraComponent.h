// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerComponentBase.h"
#include "MPlayerCameraComponent.generated.h"


class UCameraComponent;
class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerCameraComponent : public UMPlayerComponentBase
{
	GENERATED_BODY()

public:
	UMPlayerCameraComponent();

protected:
	virtual void BeginPlay() override;
	
private:
	void InitializeCameraComponents();

private:
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;
};

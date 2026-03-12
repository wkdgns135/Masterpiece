// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MPlayerInputDeveloperSettings.generated.h"

class UMPlayerInputConfig;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Player Input"))
class MASTERPIECE_API UMPlayerInputDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override;

	const UMPlayerInputConfig* GetDefaultInputConfig() const;

private:
	UPROPERTY(Config, EditAnywhere, Category="Input")
	TSoftObjectPtr<UMPlayerInputConfig> DefaultInputConfig;
};

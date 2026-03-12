// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerInputDeveloperSettings.h"

#include "Gameplay/Character/Player/Input/MPlayerInputConfig.h"

FName UMPlayerInputDeveloperSettings::GetCategoryName() const
{
	return TEXT("Game");
}

const UMPlayerInputConfig* UMPlayerInputDeveloperSettings::GetDefaultInputConfig() const
{
	return DefaultInputConfig.IsNull() ? nullptr : DefaultInputConfig.LoadSynchronous();
}

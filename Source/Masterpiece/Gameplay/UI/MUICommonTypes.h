// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUICommonTypes.generated.h"

UENUM(BlueprintType)
enum class EMUIInputPolicy : uint8
{
	Game,
	GameAndMenu,
	Menu
};

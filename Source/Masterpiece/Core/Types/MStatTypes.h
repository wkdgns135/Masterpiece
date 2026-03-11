// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MStatTypes.generated.h"

USTRUCT(BlueprintType)
struct FBaseStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float AttackPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Base")
	float DefensePower = 0.0f;
};

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	FBaseStat BaseStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	float CurrentMana = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	int32 Experience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	int32 SkillPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Player")
	int32 Gold = 0;
};

USTRUCT(BlueprintType)
struct FEnemyStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Enemy")
	FBaseStat BaseStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Enemy")
	float AggroRange = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Enemy")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Enemy")
	int32 RewardExperience = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat|Enemy")
	int32 RewardGold = 0;
};

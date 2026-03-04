// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MItemTypes.generated.h"

UENUM(BlueprintType)
enum class EMItemType : uint8
{
	Artifact,
	Consumable,
	Quest,
	Material
};

UENUM(BlueprintType)
enum class EMArtifactType : uint8
{
	Weapon,
	Equipment,
	Accessory
};

UENUM(BlueprintType)
enum class EMItemRarity : uint8
{
	Common,
	Rare,
	Unique,
	Legendary,
	Epic,
	Masterpiece
};

UENUM(BlueprintType)
enum class EMConsumableType : uint8
{
	HealthPotion,
	ManaPotion,
	BuffPotion,
	ThrowItem
};

USTRUCT(BlueprintType)
struct FItemStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName StatName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float Value = 0.0f;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EMItemType ItemType = EMItemType::Artifact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EMItemRarity Rarity = EMItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 MaxStackCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 BuyPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 SellPrice = 0;
};

USTRUCT(BlueprintType)
struct FArtifactData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Artifact")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Artifact")
	EMArtifactType ArtifactType = EMArtifactType::Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Artifact")
	FName ThemeId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Artifact")
	TArray<FItemStatModifier> StatModifiers;
};

USTRUCT(BlueprintType)
struct FConsumableData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
	EMConsumableType ConsumableType = EMConsumableType::HealthPotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
	float Cooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
	TArray<FItemStatModifier> Effects;
};

USTRUCT(BlueprintType)
struct FInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 Count = 1;
};

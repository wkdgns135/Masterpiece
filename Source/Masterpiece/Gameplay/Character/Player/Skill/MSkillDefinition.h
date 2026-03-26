// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MSkillDefinition.generated.h"

class UAnimMontage;
class UTexture2D;
class UMPlayerAbility_Skill;

USTRUCT(BlueprintType)
struct FMSkillPrerequisite
{
	GENERATED_BODY()

	// 선행 스킬 태그입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FGameplayTag SkillTag;

	// 선행 스킬의 요구 랭크입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill", meta=(ClampMin="1", UIMin="1"))
	int32 RequiredRank = 1;
};

USTRUCT(BlueprintType)
struct FMSkillDefinitionBase
{
	GENERATED_BODY()

	// 스킬 UI에 표시할 아이콘 에셋입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|UI")
	TSoftObjectPtr<UTexture2D> Icon;

	// 스킬 고유 식별 태그입니다. 런타임/세이브 키로 사용합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Identity")
	FGameplayTag SkillTag;

	// 스킬 UI에 표시할 이름입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|UI")
	FText DisplayName;

	// 스킬 UI에 표시할 설명입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|UI", meta=(MultiLine="true"))
	FText Description;

	// 선행 스킬 조건 목록입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Requirement")
	TArray<FMSkillPrerequisite> Prerequisites;

	// 스킬을 배우기 위한 최소 캐릭터 레벨입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Requirement", meta=(ClampMin="1", UIMin="1"))
	int32 RequiredCharacterLevel = 1;

	// 스킬의 최대 랭크입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Progression", meta=(ClampMin="1", UIMin="1"))
	int32 MaxRank = 1;

	// 랭크 1회 투자 시 소비되는 스킬 포인트입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Progression", meta=(ClampMin="0", UIMin="0"))
	int32 CostPerRank = 1;
};

USTRUCT(BlueprintType)
struct FMSkillDefinitionActive : public FMSkillDefinitionBase
{
	GENERATED_BODY()

	// 시전 준비 단계에서 재생할 몽타주 에셋입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation")
	TSoftObjectPtr<UAnimMontage> CastingMontage;

	// 스킬 본 시전에서 재생할 몽타주 에셋입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation")
	TSoftObjectPtr<UAnimMontage> SkillMontage;

	// 슬롯 장착 시 부여할 스킬 어빌리티 클래스입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Identity")
	TSoftClassPtr<UMPlayerAbility_Skill> AbilityClass;

	// 스킬 실행 라우팅 태그입니다. 예: Ability.Skill.Q
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Identity")
	FGameplayTag AbilityTag;

	// 몽타주 섹션명입니다. None이면 기본 재생입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation")
	FName MontageSection = NAME_None;

	// 랭크 1 기준 데미지 배율입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat")
	float DamageMultiplierBase = 1.0f;

	// 랭크 증가 시 추가되는 데미지 배율입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat")
	float DamageMultiplierPerRank = 0.1f;

	// 랭크 1 기준 쿨다운(초)입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(ClampMin="0.0", UIMin="0.0"))
	float CooldownSecondsBase = 0.0f;

	// 랭크 증가 시 추가되는 쿨다운 변화량(초)입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat")
	float CooldownSecondsPerRank = 0.0f;

	// 랭크 1 기준 마나 소모량입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(ClampMin="0.0", UIMin="0.0"))
	float ManaCostBase = 0.0f;

	// 랭크 증가 시 추가되는 마나 소모량입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat")
	float ManaCostPerRank = 0.0f;

	// 시전 시간(초)입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(ClampMin="0.0", UIMin="0.0"))
	float CastTimeSeconds = 0.0f;
};

USTRUCT(BlueprintType)
struct FMSkillDefinitionPassive : public FMSkillDefinitionBase
{
	GENERATED_BODY()

	// 패시브 효과 식별 태그입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Passive")
	FGameplayTag PassiveEffectTag;

	// 랭크 1 기준 패시브 효과값입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Passive")
	float EffectValueBase = 0.0f;

	// 랭크 증가 시 추가되는 패시브 효과값입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Passive")
	float EffectValuePerRank = 0.0f;
};

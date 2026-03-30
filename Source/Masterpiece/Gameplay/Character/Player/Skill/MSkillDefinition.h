#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Definition/MDefinitionObject.h"
#include "MSkillDefinition.generated.h"

class UAnimMontage;
class UTexture2D;
class UMPlayerAbility_Skill;

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class MASTERPIECE_API UMSkillDefinition : public UMDefinitionObject
{
	GENERATED_BODY()

public:
	FGameplayTag GetSkillTag() const;
	const TSoftObjectPtr<UTexture2D>& GetIcon() const;
	const FText& GetDisplayName() const;
	const FText& GetDescription() const;
	const TMap<FGameplayTag, int32>& GetPrerequisiteSkillRanks() const;
	TArray<FGameplayTag> GetPrerequisiteSkillTags() const;
	int32 GetRequiredCharacterLevel() const;
	int32 GetMaxRank() const;
	int32 GetCostPerRank() const;

	virtual bool IsPassiveDefinition() const PURE_VIRTUAL(UMSkillDefinition::IsPassiveDefinition, return false;);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|UI", meta=(ToolTip="스킬 UI에 표시할 아이콘입니다."))
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|UI", meta=(ToolTip="스킬 UI에 표시할 이름입니다."))
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|UI", meta=(MultiLine="true", ToolTip="스킬 상세 UI에 표시할 설명입니다."))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Requirement", meta=(ToolTip="선행 스킬과 필요한 랭크를 지정합니다. Key는 선행 스킬 태그, Value는 필요한 랭크입니다."))
	TMap<FGameplayTag, int32> PrerequisiteSkillRanks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Requirement", meta=(ClampMin="1", UIMin="1", ToolTip="이 스킬을 해금하거나 투자하기 위해 필요한 최소 캐릭터 레벨입니다."))
	int32 RequiredCharacterLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Progression", meta=(ClampMin="1", UIMin="1", ToolTip="이 스킬이 가질 수 있는 최대 랭크입니다."))
	int32 MaxRank = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Progression", meta=(ClampMin="0", UIMin="0", ToolTip="랭크 1회 투자에 필요한 비용입니다."))
	int32 CostPerRank = 1;
};

UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillDefinitionActive : public UMSkillDefinition
{
	GENERATED_BODY()

public:
	const TSoftObjectPtr<UAnimMontage>& GetCastingMontage() const;
	const TSoftObjectPtr<UAnimMontage>& GetSkillMontage() const;
	const TSoftClassPtr<UMPlayerAbility_Skill>& GetAbilityClass() const;
	FGameplayTag GetAbilityTag() const;
	FName GetMontageSection() const;
	float GetDamageMultiplierBase() const;
	float GetDamageMultiplierPerRank() const;
	float GetCooldownSecondsBase() const;
	float GetCooldownSecondsPerRank() const;
	float GetManaCostBase() const;
	float GetManaCostPerRank() const;
	float GetCastTimeSeconds() const;

	virtual bool IsPassiveDefinition() const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation", meta=(AllowPrivateAccess="true", ToolTip="시전 준비 단계에서 재생할 몽타주입니다."))
	TSoftObjectPtr<UAnimMontage> CastingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation", meta=(AllowPrivateAccess="true", ToolTip="실제 스킬 사용 시 재생할 몽타주입니다."))
	TSoftObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Identity", meta=(AllowPrivateAccess="true", ToolTip="이 스킬에 직접 부여할 게임플레이 어빌리티 클래스입니다."))
	TSoftClassPtr<UMPlayerAbility_Skill> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Identity", meta=(AllowPrivateAccess="true", ToolTip="스킬 사용과 입력 연계에 사용할 어빌리티 태그입니다. 예: Ability.Skill.Q"))
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation", meta=(AllowPrivateAccess="true", ToolTip="몽타주 재생 시작 섹션 이름입니다. None이면 기본 시작점을 사용합니다."))
	FName MontageSection = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ToolTip="랭크 1 기준 기본 피해 배율입니다."))
	float DamageMultiplierBase = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ToolTip="랭크가 1 증가할 때마다 추가되는 피해 배율입니다."))
	float DamageMultiplierPerRank = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ClampMin="0.0", UIMin="0.0", ToolTip="랭크 1 기준 기본 쿨다운 시간(초)입니다."))
	float CooldownSecondsBase = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ToolTip="랭크가 1 증가할 때마다 추가되는 쿨다운 변화량(초)입니다."))
	float CooldownSecondsPerRank = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ClampMin="0.0", UIMin="0.0", ToolTip="랭크 1 기준 기본 마나 소모량입니다."))
	float ManaCostBase = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ToolTip="랭크가 1 증가할 때마다 추가되는 마나 소모 변화량입니다."))
	float ManaCostPerRank = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Combat", meta=(AllowPrivateAccess="true", ClampMin="0.0", UIMin="0.0", ToolTip="스킬 시전에 필요한 시간(초)입니다."))
	float CastTimeSeconds = 0.0f;
};

UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillDefinitionPassive : public UMSkillDefinition
{
	GENERATED_BODY()

public:
	FGameplayTag GetPassiveEffectTag() const;
	float GetEffectValueBase() const;
	float GetEffectValuePerRank() const;

	virtual bool IsPassiveDefinition() const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Passive", meta=(AllowPrivateAccess="true", ToolTip="패시브 효과를 식별하는 태그입니다."))
	FGameplayTag PassiveEffectTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Passive", meta=(AllowPrivateAccess="true", ToolTip="랭크 1 기준 패시브 효과 기본값입니다."))
	float EffectValueBase = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Passive", meta=(AllowPrivateAccess="true", ToolTip="랭크가 1 증가할 때마다 추가되는 패시브 효과값입니다."))
	float EffectValuePerRank = 0.0f;
};

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/Tooltip/MDefinitionTooltipWidget.h"
#include "MSkillTooltipWidget.generated.h"

class UBorder;
class UImage;
class UMSkillInstance;
class USizeBox;
class UTextBlock;
class UVerticalBox;

UCLASS(Blueprintable)
class MASTERPIECE_API UMSkillTooltipWidget : public UMDefinitionTooltipWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI|Tooltip|Skill")
	UMSkillInstance* GetSkillInstance() const;

protected:
	virtual void NativeConstruct() override;
	virtual void HandleDefinitionInstanceSet() override;

	UFUNCTION(BlueprintImplementableEvent, Category="UI|Tooltip|Skill")
	void K2_OnSkillInstanceSet(UMSkillInstance* InSkillInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillName;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillDescription;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillRank;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillRequirement;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillCost;

	UPROPERTY(BlueprintReadOnly, Category="UI|Tooltip|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillState;

private:
	void EnsureDefaultLayout();
	void RefreshSkillTooltip();
	FText BuildRankText(const UMSkillInstance* InSkillInstance) const;
	FText BuildRequirementText(const UMSkillInstance* InSkillInstance) const;
	FText BuildCostText(const UMSkillInstance* InSkillInstance) const;
	FText BuildStateText(const UMSkillInstance* InSkillInstance) const;
};

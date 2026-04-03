#include "Gameplay/UI/Skill/MSkillTooltipWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"

#define LOCTEXT_NAMESPACE "MSkillTooltipWidget"

UMSkillInstance* UMSkillTooltipWidget::GetSkillInstance() const
{
	return Cast<UMSkillInstance>(GetDefinitionInstance());
}

void UMSkillTooltipWidget::NativeConstruct()
{
	EnsureDefaultLayout();

	Super::NativeConstruct();

	EnsureDefaultLayout();
	RefreshSkillTooltip();
}

void UMSkillTooltipWidget::HandleDefinitionInstanceSet()
{
	EnsureDefaultLayout();
	RefreshSkillTooltip();
	K2_OnSkillInstanceSet(GetSkillInstance());
}

void UMSkillTooltipWidget::EnsureDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UBorder* RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("RootBorder"));
	USizeBox* RootSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("RootSizeBox"));
	UVerticalBox* RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootBox"));
	UHorizontalBox* HeaderBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HeaderBox"));
	UVerticalBox* HeaderTextBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HeaderTextBox"));

	SkillIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("SkillIcon"));
	SkillName = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkillName"));
	SkillState = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkillState"));
	SkillDescription = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkillDescription"));
	SkillRank = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkillRank"));
	SkillRequirement = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkillRequirement"));
	SkillCost = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkillCost"));

	WidgetTree->RootWidget = RootBorder;

	RootBorder->SetBrushColor(FLinearColor(0.04f, 0.06f, 0.08f, 0.96f));
	RootBorder->SetPadding(FMargin(16.0f));
	RootBorder->SetContent(RootSizeBox);

	RootSizeBox->SetWidthOverride(320.0f);
	RootSizeBox->SetContent(RootBox);

	if (UVerticalBoxSlot* HeaderSlot = RootBox->AddChildToVerticalBox(HeaderBox))
	{
		HeaderSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	}

	SkillIcon->SetBrushSize(FVector2D(48.0f, 48.0f));
	if (UHorizontalBoxSlot* IconSlot = HeaderBox->AddChildToHorizontalBox(SkillIcon))
	{
		IconSlot->SetPadding(FMargin(0.0f, 0.0f, 12.0f, 0.0f));
		IconSlot->SetVerticalAlignment(VAlign_Top);
		IconSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
	}

	if (UHorizontalBoxSlot* HeaderTextSlot = HeaderBox->AddChildToHorizontalBox(HeaderTextBox))
	{
		HeaderTextSlot->SetHorizontalAlignment(HAlign_Fill);
		HeaderTextSlot->SetVerticalAlignment(VAlign_Center);
		HeaderTextSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	SkillName->SetColorAndOpacity(FSlateColor(FLinearColor(0.95f, 0.97f, 1.0f, 1.0f)));
	SkillName->SetAutoWrapText(true);
	if (UVerticalBoxSlot* NameSlot = HeaderTextBox->AddChildToVerticalBox(SkillName))
	{
		NameSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
	}

	SkillState->SetColorAndOpacity(FSlateColor(FLinearColor(0.60f, 0.78f, 1.0f, 1.0f)));
	SkillState->SetAutoWrapText(true);
	HeaderTextBox->AddChildToVerticalBox(SkillState);

	SkillDescription->SetColorAndOpacity(FSlateColor(FLinearColor(0.88f, 0.90f, 0.93f, 1.0f)));
	SkillDescription->SetAutoWrapText(true);
	if (UVerticalBoxSlot* DescriptionSlot = RootBox->AddChildToVerticalBox(SkillDescription))
	{
		DescriptionSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	}

	SkillRank->SetColorAndOpacity(FSlateColor(FLinearColor(0.76f, 0.84f, 0.92f, 1.0f)));
	if (UVerticalBoxSlot* RankSlot = RootBox->AddChildToVerticalBox(SkillRank))
	{
		RankSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
	}

	SkillRequirement->SetColorAndOpacity(FSlateColor(FLinearColor(0.76f, 0.84f, 0.92f, 1.0f)));
	if (UVerticalBoxSlot* RequirementSlot = RootBox->AddChildToVerticalBox(SkillRequirement))
	{
		RequirementSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
	}

	SkillCost->SetColorAndOpacity(FSlateColor(FLinearColor(0.76f, 0.84f, 0.92f, 1.0f)));
	RootBox->AddChildToVerticalBox(SkillCost);
}

void UMSkillTooltipWidget::RefreshSkillTooltip()
{
	const UMSkillInstance* SkillInstance = GetSkillInstance();
	if (!SkillInstance)
	{
		if (SkillIcon)
		{
			SkillIcon->SetBrushFromTexture(nullptr);
		}
		if (SkillName)
		{
			SkillName->SetText(FText::GetEmpty());
		}
		if (SkillDescription)
		{
			SkillDescription->SetText(FText::GetEmpty());
		}
		if (SkillRank)
		{
			SkillRank->SetText(FText::GetEmpty());
		}
		if (SkillRequirement)
		{
			SkillRequirement->SetText(FText::GetEmpty());
		}
		if (SkillCost)
		{
			SkillCost->SetText(FText::GetEmpty());
		}
		if (SkillState)
		{
			SkillState->SetText(FText::GetEmpty());
		}
		return;
	}

	if (SkillIcon)
	{
		SkillIcon->SetBrushFromTexture(SkillInstance->GetIcon().LoadSynchronous());
	}
	if (SkillName)
	{
		SkillName->SetText(SkillInstance->GetDisplayName());
	}
	if (SkillDescription)
	{
		SkillDescription->SetText(SkillInstance->GetDescription());
	}
	if (SkillRank)
	{
		SkillRank->SetText(BuildRankText(SkillInstance));
	}
	if (SkillRequirement)
	{
		SkillRequirement->SetText(BuildRequirementText(SkillInstance));
	}
	if (SkillCost)
	{
		SkillCost->SetText(BuildCostText(SkillInstance));
	}
	if (SkillState)
	{
		SkillState->SetText(BuildStateText(SkillInstance));
	}
}

FText UMSkillTooltipWidget::BuildRankText(const UMSkillInstance* InSkillInstance) const
{
	return InSkillInstance
		? FText::Format(
			LOCTEXT("SkillRankFormat", "Rank {0}/{1}"),
			FText::AsNumber(InSkillInstance->GetCurrentRank()),
			FText::AsNumber(InSkillInstance->GetMaxRank()))
		: FText::GetEmpty();
}

FText UMSkillTooltipWidget::BuildRequirementText(const UMSkillInstance* InSkillInstance) const
{
	return InSkillInstance
		? FText::Format(
			LOCTEXT("SkillRequirementFormat", "Required Level {0}"),
			FText::AsNumber(InSkillInstance->GetRequiredCharacterLevel()))
		: FText::GetEmpty();
}

FText UMSkillTooltipWidget::BuildCostText(const UMSkillInstance* InSkillInstance) const
{
	return InSkillInstance
		? FText::Format(
			LOCTEXT("SkillCostFormat", "Cost Per Rank {0}"),
			FText::AsNumber(InSkillInstance->GetCostPerRank()))
		: FText::GetEmpty();
}

FText UMSkillTooltipWidget::BuildStateText(const UMSkillInstance* InSkillInstance) const
{
	if (!InSkillInstance)
	{
		return FText::GetEmpty();
	}

	if (InSkillInstance->IsEquipped())
	{
		return LOCTEXT("SkillStateEquipped", "Equipped");
	}

	if (InSkillInstance->IsPassive())
	{
		return LOCTEXT("SkillStatePassive", "Passive");
	}

	if (!InSkillInstance->IsUnlocked())
	{
		return LOCTEXT("SkillStateLocked", "Locked");
	}

	return LOCTEXT("SkillStateActive", "Active");
}

#undef LOCTEXT_NAMESPACE

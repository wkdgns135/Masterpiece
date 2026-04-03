#include "Gameplay/UI/Skill/MSkillTreeSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "Gameplay/UI/Skill/MSkillTooltipWidget.h"

UMSkillTreeSlotWidget::UMSkillTreeSlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefinitionToolTipWidgetClass = UMSkillTooltipWidget::StaticClass();
}

void UMSkillTreeSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	RefreshWidget();
}

void UMSkillTreeSlotWidget::NativeRefreshWidget()
{
	const UMSkillInstance* SkillInstance = GetSkillInstance();
	if (!SkillInstance)
	{
		if (SkillIcon)
		{
			SkillIcon->SetBrushFromTexture(nullptr);
		}
		if (SkillRank)
		{
			SkillRank->SetText(FText::GetEmpty());
		}
		return;
	}
	
	if (SkillIcon)
	{
		SkillIcon->SetBrushFromTexture(SkillInstance->GetIcon().LoadSynchronous());
	}
	if (SkillRank)
	{
		SkillRank->SetText(FText::AsNumber(SkillInstance->GetCurrentRank()));
	}
}

UMSkillInstance* UMSkillTreeSlotWidget::GetSkillInstance() const
{
	return Cast<UMSkillInstance>(GetDefinitionInstance());
}

UImage* UMSkillTreeSlotWidget::GetDragHandleImage_Implementation() const
{
	return SkillIcon.Get();
}

bool UMSkillTreeSlotWidget::CanDragDefinitionInstance_Implementation() const
{
	const UMSkillInstance* SkillInstance = GetSkillInstance();
	if (SkillInstance == nullptr)
	{
		return false;
	}
	if (SkillInstance->IsPassive())
	{
		return false;
	}
	if (SkillInstance->GetCurrentRank() < 1)
	{
		return false;
	}
	
	return true;
}

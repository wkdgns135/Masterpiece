#include "Gameplay/UI/Tooltip/MDefinitionTooltipWidget.h"

#include "Gameplay/Definition/MDefinitionInstance.h"
#include "Layout/Visibility.h"

void UMDefinitionTooltipWidget::SetDefinitionInstance(UMDefinitionInstance* InDefinitionInstance)
{
	DefinitionInstance = InDefinitionInstance;
	HandleDefinitionInstanceSet();
	K2_OnDefinitionInstanceSet(DefinitionInstance);
}

UMDefinitionInstance* UMDefinitionTooltipWidget::GetDefinitionInstance() const
{
	return DefinitionInstance.Get();
}

void UMDefinitionTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::HitTestInvisible);

	if (DefinitionInstance)
	{
		HandleDefinitionInstanceSet();
	}
}

void UMDefinitionTooltipWidget::HandleDefinitionInstanceSet()
{
}

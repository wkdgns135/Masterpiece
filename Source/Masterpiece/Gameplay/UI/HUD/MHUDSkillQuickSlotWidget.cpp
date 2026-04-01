// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/HUD/MHUDSkillQuickSlotWidget.h"

#include "Components/Image.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "Gameplay/Definition/MDefinitionInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogMHUDSkillQuickSlotWidget, Log, All);

UMPlayerSkillComponent* UMHUDSkillQuickSlotWidget::GetSkillComponent() const
{
	return BoundSkillComponent.Get();
}

UMSkillInstance* UMHUDSkillQuickSlotWidget::GetSkillInstance() const
{
	return Cast<UMSkillInstance>(GetDefinitionInstance());
}

FGameplayTag UMHUDSkillQuickSlotWidget::GetResolvedSkillSlotTag() const
{
	return GetSlotTag().IsValid() ? GetSlotTag() : ResolveDefaultSkillSlotTagFromIndex();
}

FGameplayTag UMHUDSkillQuickSlotWidget::GetAssignedSkillTag() const
{
	const UMPlayerSkillComponent* SkillComponent = GetSkillComponent();
	const FGameplayTag SkillSlotTag = GetResolvedSkillSlotTag();
	return SkillComponent && SkillSlotTag.IsValid() ? SkillComponent->GetAssignedSkillTag(SkillSlotTag) : FGameplayTag();
}

bool UMHUDSkillQuickSlotWidget::HasAssignedSkill() const
{
	return GetAssignedSkillTag().IsValid();
}

void UMHUDSkillQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMHUDSkillQuickSlotWidget::NativeDestruct()
{
	UnbindSkillComponent();
	BoundSkillComponent = nullptr;

	Super::NativeDestruct();
}

bool UMHUDSkillQuickSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const bool bHandledDrop = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (bHandledDrop)
	{
		UE_LOG(LogMHUDSkillQuickSlotWidget, Log, TEXT("Skill quick slot drop handled: SlotTag=%s, SlotIndex=%d"),
			*GetResolvedSkillSlotTag().ToString(), GetSlotIndex());
		return true;
	}

	UE_LOG(LogMHUDSkillQuickSlotWidget, Log, TEXT("Skill quick slot drop ignored: SlotTag=%s, SlotIndex=%d"),
		*GetResolvedSkillSlotTag().ToString(), GetSlotIndex());

	return bHandledDrop;
}

void UMHUDSkillQuickSlotWidget::HandleOwningPlayerCharacterChanged()
{
	Super::HandleOwningPlayerCharacterChanged();

	UnbindSkillComponent();
	BoundSkillComponent = nullptr;

	if (const AMPlayerCharacterBase* PlayerCharacter = GetBoundPlayerCharacter())
	{
		BoundSkillComponent = PlayerCharacter->GetSkillComponent();
	}

	BindSkillComponent();
}

UMDefinitionInstance* UMHUDSkillQuickSlotWidget::ResolveQuickSlotDefinitionInstance() const
{
	const UMPlayerSkillComponent* SkillComponent = GetSkillComponent();
	const FGameplayTag SkillSlotTag = GetResolvedSkillSlotTag();
	if (!SkillComponent || !SkillSlotTag.IsValid())
	{
		return nullptr;
	}

	const FGameplayTag SkillTag = SkillComponent->GetAssignedSkillTag(SkillSlotTag);
	return SkillTag.IsValid() ? SkillComponent->GetSkillInstance(SkillTag) : nullptr;
}

UImage* UMHUDSkillQuickSlotWidget::GetDragHandleImage_Implementation() const
{
	return SkillIcon.Get();
}

UMDefinitionInstance* UMHUDSkillQuickSlotWidget::GetDraggableDefinitionInstance_Implementation() const
{
	return GetDefinitionInstance();
}

bool UMHUDSkillQuickSlotWidget::CanDragDefinitionInstance_Implementation() const
{
	return GetDefinitionInstance() != nullptr;
}

bool UMHUDSkillQuickSlotWidget::CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const
{
	const UMSkillInstance* SkillInstance = Cast<UMSkillInstance>(DefinitionInstance);
	return GetSkillComponent() && SkillInstance && SkillInstance->GetSkillTag().IsValid() && GetResolvedSkillSlotTag().IsValid();
}

bool UMHUDSkillQuickSlotWidget::DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance)
{
	UMPlayerSkillComponent* SkillComponent = GetSkillComponent();
	UMSkillInstance* SkillInstance = Cast<UMSkillInstance>(DefinitionInstance);
	const FGameplayTag ResolvedSlotTag = GetResolvedSkillSlotTag();
	if (!SkillComponent || !SkillInstance || !ResolvedSlotTag.IsValid())
	{
		UE_LOG(LogMHUDSkillQuickSlotWidget, Warning, TEXT("Skill quick slot drop rejected: SlotTag=%s, SkillInstance=%s, SkillComponent=%s"),
			*ResolvedSlotTag.ToString(),
			SkillInstance ? *SkillInstance->GetName() : TEXT("None"),
			SkillComponent ? *SkillComponent->GetName() : TEXT("None"));
		return false;
	}

	const bool bEquipped = SkillComponent->EquipSkillToSlot(SkillInstance->GetSkillTag(), ResolvedSlotTag, SkillInstance->GetCurrentRank());
	UE_LOG(LogMHUDSkillQuickSlotWidget, Log, TEXT("Skill quick slot drop result: SlotTag=%s, SkillTag=%s, Rank=%d, Success=%s"),
		*ResolvedSlotTag.ToString(),
		*SkillInstance->GetSkillTag().ToString(),
		SkillInstance->GetCurrentRank(),
		bEquipped ? TEXT("true") : TEXT("false"));
	return bEquipped;
}

void UMHUDSkillQuickSlotWidget::BindSkillComponent()
{
	if (BoundSkillComponent)
	{
		BoundSkillComponent->OnSkillStateChanged.AddUObject(this, &ThisClass::HandleSkillStateChanged);
	}
}

void UMHUDSkillQuickSlotWidget::UnbindSkillComponent()
{
	if (BoundSkillComponent)
	{
		BoundSkillComponent->OnSkillStateChanged.RemoveAll(this);
	}
}

void UMHUDSkillQuickSlotWidget::HandleSkillStateChanged()
{
	RefreshQuickSlot();
}

FGameplayTag UMHUDSkillQuickSlotWidget::ResolveDefaultSkillSlotTagFromIndex() const
{
	if (!bUseDefaultSkillSlotTagFromIndex)
	{
		return FGameplayTag();
	}

	switch (GetSlotIndex())
	{
	case 0:
		return MGameplayTags::Ability_Skill_Q;
	case 1:
		return MGameplayTags::Ability_Skill_W;
	case 2:
		return MGameplayTags::Ability_Skill_E;
	case 3:
		return MGameplayTags::Ability_Skill_R;
	default:
		return FGameplayTag();
	}
}

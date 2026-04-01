// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/HUD/MHUDQuickSlotWidgetBase.h"

#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Definition/MDefinitionInstance.h"

FGameplayTag UMHUDQuickSlotWidgetBase::GetSlotTag() const
{
	return SlotTag;
}

int32 UMHUDQuickSlotWidgetBase::GetSlotIndex() const
{
	return SlotIndex;
}

void UMHUDQuickSlotWidgetBase::SetSlotTag(const FGameplayTag InSlotTag)
{
	SlotTag = InSlotTag;
	RefreshQuickSlot();
}

void UMHUDQuickSlotWidgetBase::SetSlotIndex(const int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
	RefreshQuickSlot();
}

void UMHUDQuickSlotWidgetBase::RefreshQuickSlot()
{
	BoundDefinitionInstance = ResolveQuickSlotDefinitionInstance();
	K2_OnQuickSlotRefreshed(BoundDefinitionInstance.Get());
}

AMPlayerCharacterBase* UMHUDQuickSlotWidgetBase::GetBoundPlayerCharacter() const
{
	return BoundPlayerCharacter.Get();
}

UMDefinitionInstance* UMHUDQuickSlotWidgetBase::GetDefinitionInstance() const
{
	return BoundDefinitionInstance.Get();
}

void UMHUDQuickSlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	BoundPlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwningPlayerPawn());
	HandleOwningPlayerCharacterChanged();
	RefreshQuickSlot();
}

void UMHUDQuickSlotWidgetBase::NativeDestruct()
{
	BoundDefinitionInstance = nullptr;
	BoundPlayerCharacter = nullptr;
	Super::NativeDestruct();
}

void UMHUDQuickSlotWidgetBase::HandleOwningPlayerCharacterChanged()
{
	K2_OnOwningPlayerCharacterChanged(BoundPlayerCharacter.Get());
}

UMDefinitionInstance* UMHUDQuickSlotWidgetBase::ResolveQuickSlotDefinitionInstance() const
{
	return nullptr;
}

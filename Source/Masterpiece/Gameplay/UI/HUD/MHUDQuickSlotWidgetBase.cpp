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

AMPlayerCharacterBase* UMHUDQuickSlotWidgetBase::GetBoundPlayerCharacter() const
{
	return BoundPlayerCharacter.Get();
}

void UMHUDQuickSlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	BoundPlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwningPlayerPawn());
	HandleOwningPlayerCharacterChanged();
}

void UMHUDQuickSlotWidgetBase::NativeDestruct()
{
	BoundPlayerCharacter = nullptr;
	Super::NativeDestruct();
}

void UMHUDQuickSlotWidgetBase::HandleOwningPlayerCharacterChanged()
{
	K2_OnOwningPlayerCharacterChanged(BoundPlayerCharacter.Get());
}

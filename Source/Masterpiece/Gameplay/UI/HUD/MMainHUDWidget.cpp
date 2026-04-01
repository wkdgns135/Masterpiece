// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/HUD/MMainHUDWidget.h"

#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/UI/MUIGameplayTags.h"

UMMainHUDWidget::UMMainHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WidgetTag = MUIGameplayTags::UI_Widget_GameHUD;
}

AMPlayerCharacterBase* UMMainHUDWidget::GetPlayerCharacter() const
{
	return BoundPlayerCharacter.Get();
}

UMPlayerSkillComponent* UMMainHUDWidget::GetSkillComponent() const
{
	const AMPlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	return PlayerCharacter ? PlayerCharacter->GetSkillComponent() : nullptr;
}

void UMMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BoundPlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwningPlayerPawn());
	K2_OnPlayerCharacterResolved(BoundPlayerCharacter.Get());
}

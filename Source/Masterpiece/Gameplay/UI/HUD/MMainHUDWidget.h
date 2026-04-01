// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/Widget/MTaggedWidget.h"
#include "MMainHUDWidget.generated.h"

class AMPlayerCharacterBase;
class UMPlayerSkillComponent;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMMainHUDWidget : public UMTaggedWidget
{
	GENERATED_BODY()

public:
	UMMainHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category="HUD")
	AMPlayerCharacterBase* GetPlayerCharacter() const;

	UFUNCTION(BlueprintPure, Category="HUD")
	UMPlayerSkillComponent* GetSkillComponent() const;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void K2_OnPlayerCharacterResolved(AMPlayerCharacterBase* InPlayerCharacter);

private:
	UPROPERTY(Transient)
	TObjectPtr<AMPlayerCharacterBase> BoundPlayerCharacter;
};

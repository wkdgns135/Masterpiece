// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/UI/Slot/MSlotWidgetBase.h"
#include "MHUDQuickSlotWidgetBase.generated.h"

class AMPlayerCharacterBase;
class UMDefinitionInstance;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMHUDQuickSlotWidgetBase : public UMSlotWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	FGameplayTag GetSlotTag() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	int32 GetSlotIndex() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	AMPlayerCharacterBase* GetBoundPlayerCharacter() const;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void HandleOwningPlayerCharacterChanged();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD|QuickSlot")
	void K2_OnOwningPlayerCharacterChanged(AMPlayerCharacterBase* InPlayerCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD|QuickSlot")
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD|QuickSlot", meta=(ClampMin="0", UIMin="0"))
	int32 SlotIndex = INDEX_NONE;

private:
	UPROPERTY(Transient)
	TObjectPtr<AMPlayerCharacterBase> BoundPlayerCharacter;
};

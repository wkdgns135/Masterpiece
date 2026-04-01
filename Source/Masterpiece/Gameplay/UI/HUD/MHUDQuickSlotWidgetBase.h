// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/UI/DragDrop/MDragDropWidget.h"
#include "MHUDQuickSlotWidgetBase.generated.h"

class AMPlayerCharacterBase;
class UMDefinitionInstance;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMHUDQuickSlotWidgetBase : public UMDragDropWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	FGameplayTag GetSlotTag() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	int32 GetSlotIndex() const;

	UFUNCTION(BlueprintCallable, Category="HUD|QuickSlot")
	void SetSlotTag(FGameplayTag InSlotTag);

	UFUNCTION(BlueprintCallable, Category="HUD|QuickSlot")
	void SetSlotIndex(int32 InSlotIndex);

	UFUNCTION(BlueprintCallable, Category="HUD|QuickSlot")
	void RefreshQuickSlot();

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	AMPlayerCharacterBase* GetBoundPlayerCharacter() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot")
	UMDefinitionInstance* GetDefinitionInstance() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void HandleOwningPlayerCharacterChanged();
	virtual UMDefinitionInstance* ResolveQuickSlotDefinitionInstance() const;

	UFUNCTION(BlueprintImplementableEvent, Category="HUD|QuickSlot")
	void K2_OnOwningPlayerCharacterChanged(AMPlayerCharacterBase* InPlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category="HUD|QuickSlot")
	void K2_OnQuickSlotRefreshed(UMDefinitionInstance* InDefinitionInstance);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD|QuickSlot")
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD|QuickSlot", meta=(ClampMin="0", UIMin="0"))
	int32 SlotIndex = INDEX_NONE;

private:
	UPROPERTY(Transient)
	TObjectPtr<AMPlayerCharacterBase> BoundPlayerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UMDefinitionInstance> BoundDefinitionInstance;
};

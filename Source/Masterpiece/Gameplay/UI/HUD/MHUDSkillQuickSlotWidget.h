// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/UI/HUD/MHUDQuickSlotWidgetBase.h"
#include "MHUDSkillQuickSlotWidget.generated.h"

class UDragDropOperation;
class UImage;
class UMDefinitionInstance;
class UMPlayerSkillComponent;
class UMSkillInstance;
class FDragDropEvent;
struct FGeometry;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMHUDSkillQuickSlotWidget : public UMHUDQuickSlotWidgetBase
{
	GENERATED_BODY()

public:
	UMHUDSkillQuickSlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot|Skill")
	UMPlayerSkillComponent* GetSkillComponent() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot|Skill")
	UMSkillInstance* GetSkillInstance() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot|Skill")
	FGameplayTag GetResolvedSkillSlotTag() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot|Skill")
	FGameplayTag GetAssignedSkillTag() const;

	UFUNCTION(BlueprintPure, Category="HUD|QuickSlot|Skill")
	bool HasAssignedSkill() const;

protected:
	virtual void NativeDestruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void HandleOwningPlayerCharacterChanged() override;

	virtual UImage* GetDragHandleImage_Implementation() const override;
	virtual bool CanDragDefinitionInstance_Implementation() const override;
	virtual bool CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const override;
	virtual bool DropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD|QuickSlot|Skill")
	bool bUseDefaultSkillSlotTagFromIndex = true;

	UPROPERTY(BlueprintReadOnly, Category="HUD|QuickSlot|Skill", meta=(BindWidgetOptional))
	TObjectPtr<UImage> SkillIcon;

private:
	FGameplayTag ResolveDefaultSkillSlotTagFromIndex() const;
	UMSkillInstance* ResolveAssignedSkillInstance() const;
	void RefreshAssignedSkillInstance();
	void BindSkillComponent();
	void UnbindSkillComponent();
	void HandleSkillStateChanged();
	void HandleSkillSlotChanged(const FGameplayTag& InSlotTag, UMSkillInstance* InSkillInstance);

private:
	UPROPERTY(Transient)
	TObjectPtr<UMPlayerSkillComponent> BoundSkillComponent;
};

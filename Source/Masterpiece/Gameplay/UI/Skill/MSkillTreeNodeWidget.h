#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/UI/DragDrop/MDragDropWidget.h"
#include "MSkillTreeNodeWidget.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
class UMDefinitionInstance;
class UMSkillInstance;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMSkillTreeNodeWidget : public UMDragDropWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SkillTree")
	void SetSkillInstance(UMSkillInstance* InSkillInstance);

	UFUNCTION(BlueprintPure, Category="SkillTree")
	UMSkillInstance* GetSkillInstance() const;

protected:
	// UserWidget
	virtual void NativePreConstruct() override;

	// DragDropWidget 
	virtual UImage* GetDragHandleImage_Implementation() const override;
	virtual UMDefinitionInstance* GetDraggableDefinitionInstance_Implementation() const override;
	virtual bool CanDragDefinitionInstance_Implementation() const override;
	virtual bool CanDropDefinitionInstance_Implementation(UMDefinitionInstance* DefinitionInstance) const override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="SkillTree")
	void K2_OnSkillInstanceSet(UMSkillInstance* InSkillInstance);

protected:
	UPROPERTY(BlueprintReadOnly, Category="SkillTree", meta=(BindWidgetOptional))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(BlueprintReadOnly, Category="SkillTree", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillName;

	UPROPERTY(BlueprintReadOnly, Category="SkillTree", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillRank;

private:
	void RefreshSkillData();
	
	UPROPERTY(Transient)
	TObjectPtr<UMSkillInstance> SkillInstance;
};

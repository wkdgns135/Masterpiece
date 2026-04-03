#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/Slot/MSlotWidgetBase.h"
#include "MSkillTreeSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UMSkillInstance;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMSkillTreeSlotWidget : public UMSlotWidgetBase
{
	GENERATED_BODY()

public:
	UMSkillTreeSlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category="SkillTree")
	UMSkillInstance* GetSkillInstance() const;

protected:
	// UserWidget
	virtual void NativePreConstruct() override;

	// SlotWidget 
	virtual void NativeRefreshWidget() override;
	virtual UImage* GetDragHandleImage_Implementation() const override;
	virtual bool CanDragDefinitionInstance_Implementation() const override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="SkillTree", meta=(BindWidgetOptional))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(BlueprintReadOnly, Category="SkillTree", meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SkillRank;

};

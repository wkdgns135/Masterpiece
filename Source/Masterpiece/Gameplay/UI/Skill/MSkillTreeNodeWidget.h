#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSkillTreeNodeWidget.generated.h"

class UMSkillInstance;
class UMSkillTreeNodeItem;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMSkillTreeNodeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SkillTree")
	void SetNodeItem(UMSkillTreeNodeItem* InNodeItem);

	UFUNCTION(BlueprintPure, Category="SkillTree")
	UMSkillTreeNodeItem* GetNodeItem() const;

	UFUNCTION(BlueprintPure, Category="SkillTree")
	UMSkillInstance* GetSkillInstance() const;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="SkillTree")
	void K2_OnNodeItemSet(UMSkillTreeNodeItem* InNodeItem);

	UFUNCTION(BlueprintImplementableEvent, Category="SkillTree")
	void K2_OnSkillInstanceSet(UMSkillInstance* InSkillInstance);

private:
	UPROPERTY(Transient)
	TObjectPtr<UMSkillTreeNodeItem> NodeItem;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSkillTreeNodeWidget.generated.h"

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

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="SkillTree")
	void K2_OnNodeItemSet(UMSkillTreeNodeItem* InNodeItem);

private:
	UPROPERTY(Transient)
	TObjectPtr<UMSkillTreeNodeItem> NodeItem;
};

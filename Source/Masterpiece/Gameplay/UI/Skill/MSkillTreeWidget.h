// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/UI/Widget/MActivatableWidget.h"
#include "MSkillTreeWidget.generated.h"

class UPanelWidget;
class UMSkillInstance;
class UMSkillTreeNodeItem;
class UMSkillTreeNodeWidget;
class UMPlayerSkillComponent;
class AMPlayerCharacterBase;
class FPaintArgs;
class FSlateRect;
class FWidgetStyle;
class FSlateWindowElementList;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMSkillTreeWidget : public UMActivatableWidget
{
	GENERATED_BODY()

public:
	UMSkillTreeWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="SkillTree")
	void InitializeForPlayerCharacter(AMPlayerCharacterBase* InPlayerCharacter);

	UFUNCTION(BlueprintCallable, Category="SkillTree")
	void RefreshSkillTreeView();

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	void BindSkillComponentEvents();
	void UnbindSkillComponentEvents();
	void HandleSkillStateChanged();
	void BuildSkillGraphModel(TArray<TObjectPtr<UMSkillTreeNodeItem>>& OutNodes);
	void BuildPreviewGraphModel(TArray<TObjectPtr<UMSkillTreeNodeItem>>& OutNodes) const;
	void LayoutSkillGraphNodes(TArray<TObjectPtr<UMSkillTreeNodeItem>>& InOutNodes, const FVector2D& AvailableSize) const;
	void SynchronizeGraphWidgets(const TArray<TObjectPtr<UMSkillTreeNodeItem>>& Nodes);
	void UpdateNodeWidgetLayout(UMSkillTreeNodeWidget* NodeWidget, const UMSkillTreeNodeItem* Node) const;
	void ClearGraphWidgets();
	FVector2D ResolveGraphCanvasSize() const;
	bool HasMeaningfulCanvasSize(const FVector2D& InSize) const;

private:
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UPanelWidget> SkillGraphCanvas;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UMSkillTreeNodeWidget> SkillTreeNodeWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Layout", meta=(AllowPrivateAccess="true", ClampMin="100.0", UIMin="100.0"))
	float NodeHorizontalSpacing = 260.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Layout", meta=(AllowPrivateAccess="true", ClampMin="80.0", UIMin="80.0"))
	float NodeVerticalSpacing = 180.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Layout", meta=(AllowPrivateAccess="true"))
	FVector2D CanvasPadding = FVector2D(80.0f, 60.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Layout", meta=(AllowPrivateAccess="true"))
	FVector2D FallbackCanvasSize = FVector2D(1920.0f, 1080.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Preview", meta=(AllowPrivateAccess="true"))
	bool bEnableDesignerPreview = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Edge", meta=(AllowPrivateAccess="true", ClampMin="0.5", UIMin="0.5"))
	float EdgeThickness = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SkillTree|Edge", meta=(AllowPrivateAccess="true"))
	FLinearColor EdgeColor = FLinearColor(0.3f, 0.8f, 1.0f, 0.8f);

	UPROPERTY(Transient)
	TObjectPtr<AMPlayerCharacterBase> BoundPlayerCharacter;

	TWeakObjectPtr<UMPlayerSkillComponent> BoundSkillComponent;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMSkillTreeNodeItem>> GraphNodes;

	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UMSkillTreeNodeWidget>> NodeWidgetByTag;
};

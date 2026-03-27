// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/Skill/MSkillTreeWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"
#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Character/Player/Skill/MSkillTreeDataAsset.h"
#include "Gameplay/UI/MUICommonTypes.h"
#include "Gameplay/UI/MUIGameplayTags.h"
#include "Gameplay/UI/Skill/MSkillTreeNodeItem.h"
#include "Gameplay/UI/Skill/MSkillTreeNodeWidget.h"
#include "Rendering/DrawElements.h"
#include <limits>

namespace
{
struct FSkillTempNode
{
	TObjectPtr<UMSkillTreeNodeItem> Item;
	float SortKey = 0.0f;
};
}

UMSkillTreeWidget::UMSkillTreeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPolicy = EMUIInputPolicy::Menu;
	WidgetTag = MUIGameplayTags::UI_Widget_GameMenu_SkillTree;
}

void UMSkillTreeWidget::InitializeForPlayerCharacter(AMPlayerCharacterBase* InPlayerCharacter)
{
	UnbindSkillComponentEvents();
	BoundPlayerCharacter = InPlayerCharacter;
	BindSkillComponentEvents();
	RefreshSkillTreeView();
}

void UMSkillTreeWidget::RefreshSkillTreeView()
{
	GraphNodes.Reset();

	if (!SkillGraphCanvas)
	{
		ClearGraphWidgets();
		return;
	}

	BuildSkillGraphModel(GraphNodes);
	if (GraphNodes.Num() <= 0)
	{
		ClearGraphWidgets();
		return;
	}

	CachedCanvasSize = ResolveGraphCanvasSize();
	LayoutSkillGraphNodes(GraphNodes, CachedCanvasSize);
	SynchronizeGraphWidgets(GraphNodes);
	InvalidateLayoutAndVolatility();
}

void UMSkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BoundPlayerCharacter)
	{
		BoundPlayerCharacter = Cast<AMPlayerCharacterBase>(GetOwningPlayerPawn());
	}

	BindSkillComponentEvents();
	RefreshSkillTreeView();
}

void UMSkillTreeWidget::NativeDestruct()
{
	UnbindSkillComponentEvents();
	ClearGraphWidgets();
	GraphNodes.Reset();
	CachedCanvasSize = FVector2D::ZeroVector;

	Super::NativeDestruct();
}

void UMSkillTreeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GraphNodes.Num() <= 0 || !SkillGraphCanvas)
	{
		return;
	}

	const FVector2D CanvasSize = ResolveGraphCanvasSize();
	if (CanvasSize.Equals(CachedCanvasSize, 1.0f))
	{
		return;
	}

	CachedCanvasSize = CanvasSize;
	LayoutSkillGraphNodes(GraphNodes, CachedCanvasSize);
	SynchronizeGraphWidgets(GraphNodes);
	Invalidate(EInvalidateWidgetReason::Paint);
}

int32 UMSkillTreeWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	for (const TObjectPtr<UMSkillTreeNodeItem>& ChildNode : GraphNodes)
	{
		if (!ChildNode || !ChildNode->SkillTag.IsValid())
		{
			continue;
		}

		const TObjectPtr<UMSkillTreeNodeWidget>* ChildWidgetPtr = NodeWidgetByTag.Find(ChildNode->SkillTag);
		if (!ChildWidgetPtr || !IsValid(*ChildWidgetPtr))
		{
			continue;
		}

		const FGeometry ChildGeometry = (*ChildWidgetPtr)->GetCachedGeometry();
		const FVector2D ChildCenterLocal = AllottedGeometry.AbsoluteToLocal(ChildGeometry.GetAbsolutePosition() + (ChildGeometry.GetLocalSize() * 0.5f));

		for (const FGameplayTag& ParentTag : ChildNode->ParentSkillTags)
		{
			const TObjectPtr<UMSkillTreeNodeWidget>* ParentWidgetPtr = NodeWidgetByTag.Find(ParentTag);
			if (!ParentWidgetPtr || !IsValid(*ParentWidgetPtr))
			{
				continue;
			}

			const FGeometry ParentGeometry = (*ParentWidgetPtr)->GetCachedGeometry();
			const FVector2D ParentCenterLocal = AllottedGeometry.AbsoluteToLocal(ParentGeometry.GetAbsolutePosition() + (ParentGeometry.GetLocalSize() * 0.5f));

			TArray<FVector2f> Points;
			Points.Add(FVector2f(ParentCenterLocal));
			Points.Add(FVector2f(ChildCenterLocal));

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				MaxLayerId + 1,
				AllottedGeometry.ToPaintGeometry(),
				Points,
				ESlateDrawEffect::None,
				EdgeColor,
				true,
				EdgeThickness);
		}
	}

	return MaxLayerId + 1;
}

void UMSkillTreeWidget::BindSkillComponentEvents()
{
	if (!BoundPlayerCharacter)
	{
		return;
	}

	UMPlayerSkillComponent* SkillComponent = BoundPlayerCharacter->GetSkillComponent();
	if (!SkillComponent)
	{
		return;
	}

	BoundSkillComponent = SkillComponent;
	SkillComponent->OnSkillLoadoutChanged.RemoveAll(this);
	SkillComponent->OnSkillLoadoutChanged.AddUObject(this, &ThisClass::HandleSkillLoadoutChanged);
}

void UMSkillTreeWidget::UnbindSkillComponentEvents()
{
	if (!BoundSkillComponent.IsValid())
	{
		return;
	}

	BoundSkillComponent->OnSkillLoadoutChanged.RemoveAll(this);
	BoundSkillComponent.Reset();
}

void UMSkillTreeWidget::HandleSkillLoadoutChanged()
{
	RefreshSkillTreeView();
}

void UMSkillTreeWidget::BuildSkillGraphModel(TArray<TObjectPtr<UMSkillTreeNodeItem>>& OutNodes)
{
	if (!BoundPlayerCharacter)
	{
		return;
	}

	const UMPlayerSkillComponent* SkillComponent = BoundPlayerCharacter->GetSkillComponent();
	const UMSkillTreeDataAsset* SkillTreeAsset = SkillComponent ? SkillComponent->GetSkillTreeAsset() : nullptr;
	if (!SkillTreeAsset)
	{
		return;
	}

	TArray<const FMSkillDefinitionBase*> Definitions;
	TArray<bool> PassiveFlags;
	if (!SkillTreeAsset->GetAllSkillDefinitions(Definitions, PassiveFlags))
	{
		return;
	}

	OutNodes.Reserve(Definitions.Num());
	TMap<FGameplayTag, TObjectPtr<UMSkillTreeNodeItem>> NodeByTag;
	NodeByTag.Reserve(Definitions.Num());

	for (int32 Index = 0; Index < Definitions.Num(); ++Index)
	{
		const FMSkillDefinitionBase* Definition = Definitions[Index];
		if (!Definition || !Definition->SkillTag.IsValid())
		{
			continue;
		}

		UMSkillTreeNodeItem* NodeItem = NewObject<UMSkillTreeNodeItem>(this);
		NodeItem->SkillTag = Definition->SkillTag;
		NodeItem->Icon = Definition->Icon;
		NodeItem->DisplayName = Definition->DisplayName;
		NodeItem->Description = Definition->Description;
		NodeItem->RequiredCharacterLevel = Definition->RequiredCharacterLevel;
		NodeItem->CostPerRank = Definition->CostPerRank;
		NodeItem->MaxRank = Definition->MaxRank;
		NodeItem->bPassive = PassiveFlags.IsValidIndex(Index) ? PassiveFlags[Index] : false;

		for (const FMSkillPrerequisite& Prerequisite : Definition->Prerequisites)
		{
			if (Prerequisite.SkillTag.IsValid())
			{
				NodeItem->ParentSkillTags.Add(Prerequisite.SkillTag);
			}
		}

		OutNodes.Add(NodeItem);
		NodeByTag.Add(NodeItem->SkillTag, NodeItem);
	}

	for (UMSkillTreeNodeItem* Node : OutNodes)
	{
		if (!Node)
		{
			continue;
		}

		for (const FGameplayTag& ParentTag : Node->ParentSkillTags)
		{
			if (TObjectPtr<UMSkillTreeNodeItem>* ParentNode = NodeByTag.Find(ParentTag))
			{
				(*ParentNode)->ChildSkillTags.Add(Node->SkillTag);
			}
		}
	}
}

void UMSkillTreeWidget::LayoutSkillGraphNodes(TArray<TObjectPtr<UMSkillTreeNodeItem>>& InOutNodes, const FVector2D& AvailableSize) const
{
	TMap<int32, TArray<FSkillTempNode>> NodesByLevel;
	TMap<FGameplayTag, float> PreviousXByTag;
	TArray<int32> SortedLevels;

	for (UMSkillTreeNodeItem* Node : InOutNodes)
	{
		if (!Node)
		{
			continue;
		}

		NodesByLevel.FindOrAdd(Node->RequiredCharacterLevel).Add({Node, 0.0f});
	}

	NodesByLevel.GetKeys(SortedLevels);
	SortedLevels.Sort();

	const float CanvasWidth = FMath::Max(0.0f, AvailableSize.X - (CanvasPadding.X * 2.0f));
	for (int32 LayerIdx = 0; LayerIdx < SortedLevels.Num(); ++LayerIdx)
	{
		const int32 Level = SortedLevels[LayerIdx];
		TArray<FSkillTempNode>& LayerNodes = NodesByLevel.FindChecked(Level);

		for (FSkillTempNode& TempNode : LayerNodes)
		{
			TempNode.SortKey = TNumericLimits<float>::Max();
			float Sum = 0.0f;
			int32 Count = 0;
			for (const FGameplayTag& ParentTag : TempNode.Item->ParentSkillTags)
			{
				if (const float* ParentX = PreviousXByTag.Find(ParentTag))
				{
					Sum += *ParentX;
					Count += 1;
				}
			}

			if (Count > 0)
			{
				TempNode.SortKey = Sum / Count;
			}
		}

		LayerNodes.Sort([](const FSkillTempNode& A, const FSkillTempNode& B)
		{
			if (!FMath::IsNearlyEqual(A.SortKey, B.SortKey))
			{
				return A.SortKey < B.SortKey;
			}
			return A.Item->SkillTag.ToString() < B.Item->SkillTag.ToString();
		});

		const int32 NodeCount = LayerNodes.Num();
		const float ContentWidth = FMath::Max(0.0f, (NodeCount - 1) * NodeHorizontalSpacing);
		const float StartX = CanvasPadding.X + FMath::Max(0.0f, (CanvasWidth - ContentWidth) * 0.5f);
		const float PosY = CanvasPadding.Y + (LayerIdx * NodeVerticalSpacing);

		for (int32 ColIdx = 0; ColIdx < NodeCount; ++ColIdx)
		{
			UMSkillTreeNodeItem* Node = LayerNodes[ColIdx].Item;
			if (!Node)
			{
				continue;
			}

			const float PosX = StartX + (ColIdx * NodeHorizontalSpacing);
			Node->LayerIndex = LayerIdx;
			Node->ColumnIndex = ColIdx;
			Node->GraphPosition = FVector2D(PosX, PosY);
			PreviousXByTag.FindOrAdd(Node->SkillTag) = PosX;
		}
	}
}

void UMSkillTreeWidget::SynchronizeGraphWidgets(const TArray<TObjectPtr<UMSkillTreeNodeItem>>& Nodes)
{
	if (!SkillGraphCanvas)
	{
		return;
	}

	if (!SkillTreeNodeWidgetClass)
	{
		ClearGraphWidgets();
		return;
	}

	TSet<FGameplayTag> ValidNodeTags;
	ValidNodeTags.Reserve(Nodes.Num());
	for (UMSkillTreeNodeItem* Node : Nodes)
	{
		if (Node && Node->SkillTag.IsValid())
		{
			ValidNodeTags.Add(Node->SkillTag);
		}
	}

	TArray<FGameplayTag> TagsToRemove;
	for (const TPair<FGameplayTag, TObjectPtr<UMSkillTreeNodeWidget>>& Pair : NodeWidgetByTag)
	{
		if (!Pair.Key.IsValid() || !ValidNodeTags.Contains(Pair.Key) || !IsValid(Pair.Value))
		{
			TagsToRemove.Add(Pair.Key);
		}
	}

	for (const FGameplayTag& TagToRemove : TagsToRemove)
	{
		if (UMSkillTreeNodeWidget* ExistingWidget = NodeWidgetByTag.FindRef(TagToRemove))
		{
			ExistingWidget->RemoveFromParent();
		}

		NodeWidgetByTag.Remove(TagToRemove);
	}

	for (UMSkillTreeNodeItem* Node : Nodes)
	{
		if (!Node || !Node->SkillTag.IsValid())
		{
			continue;
		}

		UMSkillTreeNodeWidget* NodeWidget = NodeWidgetByTag.FindRef(Node->SkillTag);
		if (!IsValid(NodeWidget))
		{
			NodeWidget = CreateWidget<UMSkillTreeNodeWidget>(GetOwningPlayer(), SkillTreeNodeWidgetClass);
		}

		if (!NodeWidget)
		{
			continue;
		}

		if (!NodeWidgetByTag.Contains(Node->SkillTag))
		{
			SkillGraphCanvas->AddChild(NodeWidget);
			NodeWidgetByTag.Add(Node->SkillTag, NodeWidget);
		}

		NodeWidget->SetNodeItem(Node);
		UpdateNodeWidgetLayout(NodeWidget, Node);
	}
}

void UMSkillTreeWidget::UpdateNodeWidgetLayout(UMSkillTreeNodeWidget* NodeWidget, const UMSkillTreeNodeItem* Node) const
{
	if (!NodeWidget || !Node)
	{
		return;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NodeWidget->Slot))
	{
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetPosition(Node->GraphPosition);
	}
}

void UMSkillTreeWidget::ClearGraphWidgets()
{
	for (const TPair<FGameplayTag, TObjectPtr<UMSkillTreeNodeWidget>>& Pair : NodeWidgetByTag)
	{
		if (IsValid(Pair.Value))
		{
			Pair.Value->RemoveFromParent();
		}
	}

	NodeWidgetByTag.Reset();
}

FVector2D UMSkillTreeWidget::ResolveGraphCanvasSize() const
{
	if (SkillGraphCanvas)
	{
		const FVector2D CanvasSize = SkillGraphCanvas->GetCachedGeometry().GetLocalSize();
		if (HasMeaningfulCanvasSize(CanvasSize))
		{
			return CanvasSize;
		}
	}

	const FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
	if (HasMeaningfulCanvasSize(WidgetSize))
	{
		return WidgetSize;
	}

	return FallbackCanvasSize;
}

bool UMSkillTreeWidget::HasMeaningfulCanvasSize(const FVector2D& InSize) const
{
	return InSize.X > KINDA_SMALL_NUMBER && InSize.Y > KINDA_SMALL_NUMBER;
}

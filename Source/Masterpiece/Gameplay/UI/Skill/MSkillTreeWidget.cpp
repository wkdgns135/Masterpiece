// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/Skill/MSkillTreeWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/PanelWidget.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "Gameplay/Test/MTestGameplayTags.h"
#include "Gameplay/UI/MUICommonTypes.h"
#include "Gameplay/UI/MUIGameplayTags.h"
#include "Gameplay/UI/Skill/MSkillTreeNodeItem.h"
#include "Gameplay/UI/Skill/MSkillTreeNodeWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Rendering/DrawElements.h"

namespace
{
struct FRootLayoutInfo
{
	FGameplayTag RootTag;
	int32 RootLevel = MAX_int32;
	int32 SubtreeSize = 0;
	int32 SortIndex = INDEX_NONE;
};

int32 CompareRootLayoutInfo(const FRootLayoutInfo& A, const FRootLayoutInfo& B)
{
	if (A.RootLevel != B.RootLevel)
	{
		return A.RootLevel < B.RootLevel ? -1 : 1;
	}

	if (A.SubtreeSize != B.SubtreeSize)
	{
		return A.SubtreeSize > B.SubtreeSize ? -1 : 1;
	}

	return A.RootTag.ToString().Compare(B.RootTag.ToString());
}

const FRootLayoutInfo* GetRootLayoutInfo(const TMap<FGameplayTag, FRootLayoutInfo>& RootLayoutByTag, const FGameplayTag& RootTag)
{
	return RootTag.IsValid() ? RootLayoutByTag.Find(RootTag) : nullptr;
}

int32 GetRootSortIndex(const TMap<FGameplayTag, FRootLayoutInfo>& RootLayoutByTag, const FGameplayTag& RootTag)
{
	if (const FRootLayoutInfo* RootLayout = GetRootLayoutInfo(RootLayoutByTag, RootTag))
	{
		return RootLayout->SortIndex;
	}

	return MAX_int32;
}

FGameplayTag ResolvePrimaryRootTag(
	const UMSkillTreeNodeItem* Node,
	const TMap<FGameplayTag, TObjectPtr<UMSkillTreeNodeItem>>& NodeByTag,
	const TMap<FGameplayTag, FRootLayoutInfo>& RootLayoutByTag,
	TMap<FGameplayTag, FGameplayTag>& PrimaryRootByTag)
{
	if (!Node || !Node->SkillTag.IsValid())
	{
		return FGameplayTag();
	}

	if (const FGameplayTag* CachedRootTag = PrimaryRootByTag.Find(Node->SkillTag))
	{
		return *CachedRootTag;
	}

	if (Node->ParentSkillTags.Num() <= 0)
	{
		PrimaryRootByTag.Add(Node->SkillTag, Node->SkillTag);
		return Node->SkillTag;
	}

	FGameplayTag BestRootTag;
	for (const FGameplayTag& ParentTag : Node->ParentSkillTags)
	{
		const TObjectPtr<UMSkillTreeNodeItem>* ParentNode = NodeByTag.Find(ParentTag);
		const FGameplayTag CandidateRootTag = ParentNode ? ResolvePrimaryRootTag(ParentNode->Get(), NodeByTag, RootLayoutByTag, PrimaryRootByTag) : FGameplayTag();
		if (!CandidateRootTag.IsValid())
		{
			continue;
		}

		if (!BestRootTag.IsValid())
		{
			BestRootTag = CandidateRootTag;
			continue;
		}

		const FRootLayoutInfo* BestRootInfo = GetRootLayoutInfo(RootLayoutByTag, BestRootTag);
		const FRootLayoutInfo* CandidateRootInfo = GetRootLayoutInfo(RootLayoutByTag, CandidateRootTag);
		if (!BestRootInfo || !CandidateRootInfo)
		{
			continue;
		}

		if (CompareRootLayoutInfo(*CandidateRootInfo, *BestRootInfo) < 0)
		{
			BestRootTag = CandidateRootTag;
		}
	}

	PrimaryRootByTag.Add(Node->SkillTag, BestRootTag);
	return BestRootTag;
}

int32 ComputeRootSubtreeSize(const FGameplayTag& RootTag, const TMap<FGameplayTag, TObjectPtr<UMSkillTreeNodeItem>>& NodeByTag)
{
	const TObjectPtr<UMSkillTreeNodeItem>* RootNode = NodeByTag.Find(RootTag);
	if (!RootNode || !IsValid(*RootNode))
	{
		return 0;
	}

	TSet<FGameplayTag> VisitedTags;
	TArray<FGameplayTag> PendingTags;
	PendingTags.Add(RootTag);

	for (int32 PendingIndex = 0; PendingIndex < PendingTags.Num(); ++PendingIndex)
	{
		const FGameplayTag CurrentTag = PendingTags[PendingIndex];
		if (!CurrentTag.IsValid() || VisitedTags.Contains(CurrentTag))
		{
			continue;
		}

		VisitedTags.Add(CurrentTag);

		const TObjectPtr<UMSkillTreeNodeItem>* CurrentNode = NodeByTag.Find(CurrentTag);
		if (!CurrentNode || !IsValid(*CurrentNode))
		{
			continue;
		}

		for (const FGameplayTag& ChildTag : (*CurrentNode)->ChildSkillTags)
		{
			if (ChildTag.IsValid() && !VisitedTags.Contains(ChildTag))
			{
				PendingTags.Add(ChildTag);
			}
		}
	}

	return VisitedTags.Num();
}

int32 ResolvePreferredColumn(
	const UMSkillTreeNodeItem* Node,
	const TMap<FGameplayTag, int32>& AssignedColumnByTag,
	const TMap<FGameplayTag, FGameplayTag>& PrimaryRootByTag,
	const TMap<FGameplayTag, FRootLayoutInfo>& RootLayoutByTag)
{
	if (!Node)
	{
		return 0;
	}

	int32 ColumnSum = 0;
	int32 ColumnCount = 0;
	for (const FGameplayTag& ParentTag : Node->ParentSkillTags)
	{
		if (const int32* ParentColumn = AssignedColumnByTag.Find(ParentTag))
		{
			ColumnSum += *ParentColumn;
			ColumnCount += 1;
		}
	}

	if (ColumnCount > 0)
	{
		return FMath::RoundToInt(static_cast<float>(ColumnSum) / static_cast<float>(ColumnCount));
	}

	const FGameplayTag* RootTag = PrimaryRootByTag.Find(Node->SkillTag);
	return RootTag ? GetRootSortIndex(RootLayoutByTag, *RootTag) : 0;
}

int32 FindBestFreeColumn(const TSet<int32>& UsedColumns, const int32 PreferredColumn, const int32 SearchLimit)
{
	int32 BestColumn = INDEX_NONE;
	int32 BestDistance = MAX_int32;

	for (int32 CandidateColumn = 0; CandidateColumn <= SearchLimit; ++CandidateColumn)
	{
		if (UsedColumns.Contains(CandidateColumn))
		{
			continue;
		}

		const int32 Distance = FMath::Abs(CandidateColumn - PreferredColumn);
		if (Distance < BestDistance || (Distance == BestDistance && (BestColumn == INDEX_NONE || CandidateColumn < BestColumn)))
		{
			BestDistance = Distance;
			BestColumn = CandidateColumn;
		}
	}

	return BestColumn == INDEX_NONE ? 0 : BestColumn;
}

FString BuildStableNodeSortKey(const UMSkillTreeNodeItem* Node)
{
	return Node ? Node->SkillTag.ToString() : FString();
}
}

UMSkillTreeWidget::UMSkillTreeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPolicy = EMUIInputPolicy::Menu;
	WidgetTag = MUIGameplayTags::UI_Widget_GameMenu_SkillTree;
}

void UMSkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime() && bEnableDesignerPreview)
	{
		RefreshSkillTreeView();
	}
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

	if (IsDesignTime() && bEnableDesignerPreview)
	{
		BuildPreviewGraphModel(GraphNodes);
	}
	else
	{
		BuildSkillGraphModel(GraphNodes);
	}

	if (GraphNodes.Num() <= 0)
	{
		ClearGraphWidgets();
		return;
	}

	const FVector2D GraphCanvasSize = ResolveGraphCanvasSize();
	LayoutSkillGraphNodes(GraphNodes, GraphCanvasSize);
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

	Super::NativeDestruct();
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
	SkillComponent->OnSkillStateChanged.RemoveAll(this);
	SkillComponent->OnSkillStateChanged.AddUObject(this, &ThisClass::HandleSkillStateChanged);
}

void UMSkillTreeWidget::UnbindSkillComponentEvents()
{
	if (!BoundSkillComponent.IsValid())
	{
		return;
	}

	BoundSkillComponent->OnSkillStateChanged.RemoveAll(this);
	BoundSkillComponent.Reset();
}

void UMSkillTreeWidget::HandleSkillStateChanged()
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
	TArray<UMSkillInstance*> SkillInstances;
	if (!SkillComponent || !SkillComponent->GetSkillInstances(SkillInstances))
	{
		return;
	}

	OutNodes.Reserve(SkillInstances.Num());

	TMap<FGameplayTag, int32> NodeIndexByTag;
	NodeIndexByTag.Reserve(SkillInstances.Num());

	for (UMSkillInstance* SkillInstance : SkillInstances)
	{
		if (!SkillInstance || !SkillInstance->GetSkillTag().IsValid())
		{
			continue;
		}

		UMSkillTreeNodeItem* NodeItem = NewObject<UMSkillTreeNodeItem>(this);
		NodeItem->SetSkillInstance(SkillInstance);

		const int32 NodeIndex = OutNodes.Add(NodeItem);
		NodeIndexByTag.Add(NodeItem->SkillTag, NodeIndex);
	}

	for (UMSkillTreeNodeItem* NodeItem : OutNodes)
	{
		if (!NodeItem)
		{
			continue;
		}

		for (const FGameplayTag& ParentTag : NodeItem->ParentSkillTags)
		{
			const int32* ParentIndex = NodeIndexByTag.Find(ParentTag);
			if (!ParentIndex || !OutNodes.IsValidIndex(*ParentIndex))
			{
				continue;
			}

			OutNodes[*ParentIndex]->ChildSkillTags.AddUnique(NodeItem->SkillTag);
		}
	}
}

void UMSkillTreeWidget::BuildPreviewGraphModel(TArray<TObjectPtr<UMSkillTreeNodeItem>>& OutNodes) const
{
	auto AddPreviewNode = [this, &OutNodes](const FGameplayTag& SkillTag, const TCHAR* Label, const int32 RequiredLevel, const TArray<FGameplayTag>& ParentTags)
	{
		UMSkillTreeNodeItem* NodeItem = NewObject<UMSkillTreeNodeItem>(const_cast<UMSkillTreeWidget*>(this));
		NodeItem->SkillTag = SkillTag;
		NodeItem->DisplayName = FText::FromString(Label);
		NodeItem->Description = FText::Format(NSLOCTEXT("SkillTreePreview", "DescriptionFormat", "{0} Preview Node"), FText::FromString(Label));
		NodeItem->RequiredCharacterLevel = RequiredLevel;
		NodeItem->CostPerRank = 1;
		NodeItem->MaxRank = 3;
		NodeItem->ParentSkillTags = ParentTags;
		NodeItem->CurrentRank = RequiredLevel >= 10 ? 2 : 1;
		NodeItem->bUnlocked = RequiredLevel <= 5;
		NodeItem->bEquipped = SkillTag == MTestGameplayTags::Test_UI_Preview_SkillTree_A2 || SkillTag == MTestGameplayTags::Test_UI_Preview_SkillTree_B2;

		OutNodes.Add(NodeItem);
		return NodeItem;
	};

	auto LinkChild = [](UMSkillTreeNodeItem* ParentNode, UMSkillTreeNodeItem* ChildNode)
	{
		if (!ParentNode || !ChildNode)
		{
			return;
		}

		ParentNode->ChildSkillTags.AddUnique(ChildNode->SkillTag);
	};

	UMSkillTreeNodeItem* A1 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_A1, TEXT("A1"), 1, {});
	UMSkillTreeNodeItem* B1 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_B1, TEXT("B1"), 1, {});
	UMSkillTreeNodeItem* C1 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_C1, TEXT("C1"), 1, {});
	UMSkillTreeNodeItem* D1 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_D1, TEXT("D1"), 1, {});

	UMSkillTreeNodeItem* A2 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_A2, TEXT("A2"), 5, {A1->SkillTag});
	UMSkillTreeNodeItem* B2 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_B2, TEXT("B2"), 5, {B1->SkillTag});
	UMSkillTreeNodeItem* C2 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_C2, TEXT("C2"), 5, {C1->SkillTag});
	UMSkillTreeNodeItem* D2 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_D2, TEXT("D2"), 5, {D1->SkillTag});
	UMSkillTreeNodeItem* E1 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_E1, TEXT("E1"), 5, {A1->SkillTag, B1->SkillTag});

	UMSkillTreeNodeItem* A3 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_A3, TEXT("A3"), 10, {A2->SkillTag});
	UMSkillTreeNodeItem* B3 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_B3, TEXT("B3"), 10, {B2->SkillTag});
	UMSkillTreeNodeItem* C3 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_C3, TEXT("C3"), 10, {C2->SkillTag});
	UMSkillTreeNodeItem* E2 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_E2, TEXT("E2"), 10, {E1->SkillTag, A2->SkillTag});
	UMSkillTreeNodeItem* F1 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_F1, TEXT("F1"), 10, {B2->SkillTag, C2->SkillTag});

	UMSkillTreeNodeItem* A4 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_A4, TEXT("A4"), 15, {A3->SkillTag, E2->SkillTag});
	UMSkillTreeNodeItem* B4 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_B4, TEXT("B4"), 15, {B3->SkillTag, F1->SkillTag});
	UMSkillTreeNodeItem* D3 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_D3, TEXT("D3"), 15, {D2->SkillTag, F1->SkillTag});
	UMSkillTreeNodeItem* E3 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_E3, TEXT("E3"), 15, {E2->SkillTag, C3->SkillTag});

	UMSkillTreeNodeItem* A5 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_A5, TEXT("A5"), 20, {A4->SkillTag});
	UMSkillTreeNodeItem* C4 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_C4, TEXT("C4"), 20, {C3->SkillTag, E3->SkillTag});
	UMSkillTreeNodeItem* D4 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_D4, TEXT("D4"), 20, {D3->SkillTag});
	UMSkillTreeNodeItem* F2 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_F2, TEXT("F2"), 20, {B4->SkillTag, E3->SkillTag});

	UMSkillTreeNodeItem* B5 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_B5, TEXT("B5"), 25, {B4->SkillTag, F2->SkillTag});
	UMSkillTreeNodeItem* C5 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_C5, TEXT("C5"), 25, {C4->SkillTag});
	UMSkillTreeNodeItem* E4 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_E4, TEXT("E4"), 25, {A5->SkillTag, C4->SkillTag, F2->SkillTag});

	UMSkillTreeNodeItem* A6 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_A6, TEXT("A6"), 30, {A5->SkillTag, E4->SkillTag});
	UMSkillTreeNodeItem* F3 = AddPreviewNode(MTestGameplayTags::Test_UI_Preview_SkillTree_F3, TEXT("F3"), 30, {B5->SkillTag, C5->SkillTag, E4->SkillTag});

	LinkChild(A1, A2);
	LinkChild(A1, E1);
	LinkChild(B1, B2);
	LinkChild(B1, E1);
	LinkChild(C1, C2);
	LinkChild(D1, D2);

	LinkChild(A2, A3);
	LinkChild(A2, E2);
	LinkChild(B2, B3);
	LinkChild(B2, F1);
	LinkChild(C2, C3);
	LinkChild(C2, F1);
	LinkChild(D2, D3);
	LinkChild(E1, E2);

	LinkChild(A3, A4);
	LinkChild(B3, B4);
	LinkChild(C3, E3);
	LinkChild(D2, D3);
	LinkChild(E2, A4);
	LinkChild(E2, E3);
	LinkChild(F1, B4);
	LinkChild(F1, D3);

	LinkChild(A4, A5);
	LinkChild(B4, F2);
	LinkChild(B4, B5);
	LinkChild(C3, C4);
	LinkChild(D3, D4);
	LinkChild(E3, C4);
	LinkChild(E3, F2);

	LinkChild(A5, E4);
	LinkChild(A5, A6);
	LinkChild(C4, C5);
	LinkChild(C4, E4);
	LinkChild(F2, B5);
	LinkChild(F2, E4);

	LinkChild(B5, F3);
	LinkChild(C5, F3);
	LinkChild(E4, A6);
	LinkChild(E4, F3);

	A3->AssignedSlotTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Skill.Q"), false);
	B3->AssignedSlotTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Skill.W"), false);
	E2->AssignedSlotTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Skill.E"), false);
	F2->AssignedSlotTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Skill.R"), false);

	C1->bPassive = true;
	E1->bPassive = true;
	D3->bPassive = true;
	E4->bPassive = true;
	A4->CurrentRank = 3;
	B4->CurrentRank = 3;
	E3->CurrentRank = 2;
	C4->CurrentRank = 2;
	F2->CurrentRank = 2;
	A5->bUnlocked = false;
	B5->bUnlocked = false;
	C5->bUnlocked = false;
	E4->bUnlocked = false;
	A6->bUnlocked = false;
	F3->bUnlocked = false;
}

void UMSkillTreeWidget::LayoutSkillGraphNodes(TArray<TObjectPtr<UMSkillTreeNodeItem>>& InOutNodes, const FVector2D& AvailableSize) const
{
	(void)AvailableSize;

	TMap<FGameplayTag, TObjectPtr<UMSkillTreeNodeItem>> NodeByTag;
	TMap<int32, TArray<TObjectPtr<UMSkillTreeNodeItem>>> NodesByLevel;
	TArray<int32> SortedLevels;
	TArray<TObjectPtr<UMSkillTreeNodeItem>> RootNodes;
	TMap<FGameplayTag, FRootLayoutInfo> RootLayoutByTag;
	TMap<FGameplayTag, FGameplayTag> PrimaryRootByTag;
	TMap<FGameplayTag, int32> AssignedColumnByTag;

	for (UMSkillTreeNodeItem* Node : InOutNodes)
	{
		if (!Node || !Node->SkillTag.IsValid())
		{
			continue;
		}

		NodeByTag.Add(Node->SkillTag, Node);
		NodesByLevel.FindOrAdd(Node->RequiredCharacterLevel).Add(Node);
		if (Node->ParentSkillTags.Num() <= 0)
		{
			RootNodes.Add(Node);
		}
	}

	NodesByLevel.GetKeys(SortedLevels);
	SortedLevels.Sort();

	for (UMSkillTreeNodeItem* RootNode : RootNodes)
	{
		if (!RootNode || !RootNode->SkillTag.IsValid())
		{
			continue;
		}

		FRootLayoutInfo RootLayout;
		RootLayout.RootTag = RootNode->SkillTag;
		RootLayout.RootLevel = RootNode->RequiredCharacterLevel;
		RootLayout.SubtreeSize = ComputeRootSubtreeSize(RootNode->SkillTag, NodeByTag);
		RootLayoutByTag.Add(RootLayout.RootTag, RootLayout);
	}

	TArray<UMSkillTreeNodeItem*> SortedRootNodes;
	SortedRootNodes.Reserve(RootNodes.Num());
	for (UMSkillTreeNodeItem* RootNode : RootNodes)
	{
		if (RootNode)
		{
			SortedRootNodes.Add(RootNode);
		}
	}

	SortedRootNodes.Sort([&RootLayoutByTag](const UMSkillTreeNodeItem& A, const UMSkillTreeNodeItem& B)
	{
		const FRootLayoutInfo* RootLayoutA = RootLayoutByTag.Find(A.SkillTag);
		const FRootLayoutInfo* RootLayoutB = RootLayoutByTag.Find(B.SkillTag);
		if (!RootLayoutA || !RootLayoutB)
		{
			return BuildStableNodeSortKey(&A) < BuildStableNodeSortKey(&B);
		}

		const int32 CompareResult = CompareRootLayoutInfo(*RootLayoutA, *RootLayoutB);
		return CompareResult < 0;
	});

	for (int32 RootIndex = 0; RootIndex < SortedRootNodes.Num(); ++RootIndex)
	{
		if (FRootLayoutInfo* RootLayout = RootLayoutByTag.Find(SortedRootNodes[RootIndex]->SkillTag))
		{
			RootLayout->SortIndex = RootIndex;
		}
	}

	for (UMSkillTreeNodeItem* Node : InOutNodes)
	{
		const FGameplayTag RootTag = ResolvePrimaryRootTag(Node, NodeByTag, RootLayoutByTag, PrimaryRootByTag);
		if (RootTag.IsValid())
		{
			PrimaryRootByTag.FindOrAdd(Node->SkillTag) = RootTag;
		}
	}

	TMap<int32, int32> RowIndexByLevel;
	for (int32 RowIndex = 0; RowIndex < SortedLevels.Num(); ++RowIndex)
	{
		RowIndexByLevel.Add(SortedLevels[RowIndex], RowIndex);
	}

	const bool bUseGridPanel = Cast<UGridPanel>(SkillGraphCanvas) != nullptr;
	const float StartX = CanvasPadding.X;
	const float StartY = CanvasPadding.Y;
	for (int32 LayerIdx = 0; LayerIdx < SortedLevels.Num(); ++LayerIdx)
	{
		const int32 Level = SortedLevels[LayerIdx];
		TArray<TObjectPtr<UMSkillTreeNodeItem>>& LayerNodes = NodesByLevel.FindChecked(Level);
		TArray<UMSkillTreeNodeItem*> SortedLayerNodes;
		SortedLayerNodes.Reserve(LayerNodes.Num());
		for (UMSkillTreeNodeItem* Node : LayerNodes)
		{
			if (Node)
			{
				SortedLayerNodes.Add(Node);
			}
		}

		SortedLayerNodes.Sort([&](const UMSkillTreeNodeItem& A, const UMSkillTreeNodeItem& B)
		{
			const FGameplayTag RootTagA = PrimaryRootByTag.FindRef(A.SkillTag);
			const FGameplayTag RootTagB = PrimaryRootByTag.FindRef(B.SkillTag);

			const int32 RootSortA = GetRootSortIndex(RootLayoutByTag, RootTagA);
			const int32 RootSortB = GetRootSortIndex(RootLayoutByTag, RootTagB);
			if (RootSortA != RootSortB)
			{
				return RootSortA < RootSortB;
			}

			const int32 PreferredColumnA = ResolvePreferredColumn(&A, AssignedColumnByTag, PrimaryRootByTag, RootLayoutByTag);
			const int32 PreferredColumnB = ResolvePreferredColumn(&B, AssignedColumnByTag, PrimaryRootByTag, RootLayoutByTag);
			if (PreferredColumnA != PreferredColumnB)
			{
				return PreferredColumnA < PreferredColumnB;
			}

			const int32 ChildCountA = A.ChildSkillTags.Num();
			const int32 ChildCountB = B.ChildSkillTags.Num();
			if (ChildCountA != ChildCountB)
			{
				return ChildCountA > ChildCountB;
			}

			return BuildStableNodeSortKey(&A) < BuildStableNodeSortKey(&B);
		});

		TSet<int32> UsedColumns;
		int32 MaxColumnForRow = SortedLayerNodes.Num();
		for (const UMSkillTreeNodeItem* Node : SortedLayerNodes)
		{
			const int32 PreferredColumn = ResolvePreferredColumn(Node, AssignedColumnByTag, PrimaryRootByTag, RootLayoutByTag);
			MaxColumnForRow = FMath::Max(MaxColumnForRow, PreferredColumn + SortedLayerNodes.Num());
		}

		for (UMSkillTreeNodeItem* Node : SortedLayerNodes)
		{
			if (!Node)
			{
				continue;
			}

			const int32 PreferredColumn = ResolvePreferredColumn(Node, AssignedColumnByTag, PrimaryRootByTag, RootLayoutByTag);
			const int32 AssignedColumn = FindBestFreeColumn(UsedColumns, PreferredColumn, MaxColumnForRow);
			const int32 AssignedRow = RowIndexByLevel.FindRef(Node->RequiredCharacterLevel);

			Node->LayerIndex = AssignedRow;
			Node->ColumnIndex = AssignedColumn;

			if (!bUseGridPanel)
			{
				const float PosX = StartX + (AssignedColumn * NodeHorizontalSpacing);
				const float PosY = StartY + (AssignedRow * NodeVerticalSpacing);
				Node->GraphPosition = FVector2D(PosX, PosY);
			}
			else
			{
				Node->GraphPosition = FVector2D::ZeroVector;
			}

			AssignedColumnByTag.FindOrAdd(Node->SkillTag) = AssignedColumn;
			UsedColumns.Add(AssignedColumn);
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
			if (IsDesignTime())
			{
				const FString SafeTagName = Node->SkillTag.ToString().Replace(TEXT("."), TEXT("_"));
				NodeWidget = WidgetTree ? WidgetTree->ConstructWidget<UMSkillTreeNodeWidget>(SkillTreeNodeWidgetClass, FName(*FString::Printf(TEXT("PreviewNode_%s"), *SafeTagName))) : nullptr;
			}
			else
			{
				NodeWidget = CreateWidget<UMSkillTreeNodeWidget>(GetOwningPlayer(), SkillTreeNodeWidgetClass);
			}
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

	if (UGridSlot* GridSlot = Cast<UGridSlot>(NodeWidget->Slot))
	{
		GridSlot->SetRow(Node->LayerIndex);
		GridSlot->SetColumn(Node->ColumnIndex);
		GridSlot->SetHorizontalAlignment(HAlign_Center);
		GridSlot->SetVerticalAlignment(VAlign_Center);
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

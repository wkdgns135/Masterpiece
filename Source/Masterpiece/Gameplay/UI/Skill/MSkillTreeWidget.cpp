// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/Skill/MSkillTreeWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/PanelWidget.h"
#include "Gameplay/Character/Player/Component/MPlayerSkillComponent.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "Gameplay/UI/MUICommonTypes.h"
#include "Gameplay/UI/MUIGameplayTags.h"
#include "Gameplay/UI/Skill/MSkillTreeNodeWidget.h"
#include "Rendering/DrawElements.h"

namespace
{
struct FNodeGroupLayoutInfo
{
	TArray<FGameplayTag> NodeTags;
	TArray<FGameplayTag> RootTags;
	int32 MinRequiredLevel = MAX_int32;
	int32 MinLocalColumn = 0;
	int32 MaxLocalColumn = 0;
};

FString BuildStableNodeSortKey(const FGameplayTag& NodeTag)
{
	return NodeTag.ToString();
}

void GatherConnectedNodeGroup(
	const FGameplayTag& StartTag,
	const TSet<FGameplayTag>& ValidNodeTags,
	const TMap<FGameplayTag, TArray<FGameplayTag>>& ParentTagsByTag,
	const TMap<FGameplayTag, TArray<FGameplayTag>>& ChildTagsByTag,
	TSet<FGameplayTag>& VisitedTags,
	TArray<FGameplayTag>& OutNodeTags)
{
	if (!StartTag.IsValid() || !ValidNodeTags.Contains(StartTag) || VisitedTags.Contains(StartTag))
	{
		return;
	}

	TArray<FGameplayTag> PendingTags;
	PendingTags.Add(StartTag);
	VisitedTags.Add(StartTag);

	for (int32 PendingIndex = 0; PendingIndex < PendingTags.Num(); ++PendingIndex)
	{
		const FGameplayTag CurrentTag = PendingTags[PendingIndex];
		OutNodeTags.Add(CurrentTag);

		auto EnqueueNeighbor = [&](const FGameplayTag& NeighborTag)
		{
			if (!NeighborTag.IsValid() || !ValidNodeTags.Contains(NeighborTag) || VisitedTags.Contains(NeighborTag))
			{
				return;
			}

			VisitedTags.Add(NeighborTag);
			PendingTags.Add(NeighborTag);
		};

		if (const TArray<FGameplayTag>* ParentTags = ParentTagsByTag.Find(CurrentTag))
		{
			for (const FGameplayTag& ParentTag : *ParentTags)
			{
				EnqueueNeighbor(ParentTag);
			}
		}

		if (const TArray<FGameplayTag>* ChildTags = ChildTagsByTag.Find(CurrentTag))
		{
			for (const FGameplayTag& ChildTag : *ChildTags)
			{
				EnqueueNeighbor(ChildTag);
			}
		}
	}
}

void BuildGroupTopologicalOrder(
	const TArray<FGameplayTag>& GroupNodeTags,
	const TMap<FGameplayTag, TArray<FGameplayTag>>& ParentTagsByTag,
	const TMap<FGameplayTag, TArray<FGameplayTag>>& ChildTagsByTag,
	const TMap<FGameplayTag, int32>& RequiredLevelByTag,
	const TMap<FGameplayTag, int32>& RootSeedColumnByTag,
	TArray<FGameplayTag>& OutNodeOrder)
{
	TSet<FGameplayTag> GroupNodeSet;
	GroupNodeSet.Reserve(GroupNodeTags.Num());
	for (const FGameplayTag& NodeTag : GroupNodeTags)
	{
		GroupNodeSet.Add(NodeTag);
	}

	TMap<FGameplayTag, int32> RemainingParentCountByTag;
	RemainingParentCountByTag.Reserve(GroupNodeTags.Num());

	TArray<FGameplayTag> ReadyTags;
	ReadyTags.Reserve(GroupNodeTags.Num());

	for (const FGameplayTag& NodeTag : GroupNodeTags)
	{
		int32 RemainingParentCount = 0;
		if (const TArray<FGameplayTag>* ParentTags = ParentTagsByTag.Find(NodeTag))
		{
			for (const FGameplayTag& ParentTag : *ParentTags)
			{
				if (GroupNodeSet.Contains(ParentTag))
				{
					RemainingParentCount += 1;
				}
			}
		}

		RemainingParentCountByTag.Add(NodeTag, RemainingParentCount);
		if (RemainingParentCount <= 0)
		{
			ReadyTags.Add(NodeTag);
		}
	}

	auto SortNodeTags = [&](TArray<FGameplayTag>& NodeTags)
	{
		NodeTags.Sort([&](const FGameplayTag& A, const FGameplayTag& B)
		{
			const int32 RequiredLevelA = RequiredLevelByTag.FindRef(A);
			const int32 RequiredLevelB = RequiredLevelByTag.FindRef(B);
			if (RequiredLevelA != RequiredLevelB)
			{
				return RequiredLevelA < RequiredLevelB;
			}

			const int32 RootSeedA = RootSeedColumnByTag.FindRef(A);
			const int32 RootSeedB = RootSeedColumnByTag.FindRef(B);
			if (RootSeedA != RootSeedB)
			{
				return RootSeedA < RootSeedB;
			}

			const int32 ChildCountA = ChildTagsByTag.FindRef(A).Num();
			const int32 ChildCountB = ChildTagsByTag.FindRef(B).Num();
			if (ChildCountA != ChildCountB)
			{
				return ChildCountA > ChildCountB;
			}

			return BuildStableNodeSortKey(A) < BuildStableNodeSortKey(B);
		});
	};

	SortNodeTags(ReadyTags);

	TSet<FGameplayTag> ProcessedTags;
	ProcessedTags.Reserve(GroupNodeTags.Num());
	OutNodeOrder.Reset();
	OutNodeOrder.Reserve(GroupNodeTags.Num());

	while (ReadyTags.Num() > 0)
	{
		const FGameplayTag CurrentTag = ReadyTags[0];
		ReadyTags.RemoveAt(0);

		if (ProcessedTags.Contains(CurrentTag))
		{
			continue;
		}

		ProcessedTags.Add(CurrentTag);
		OutNodeOrder.Add(CurrentTag);

		if (const TArray<FGameplayTag>* ChildTags = ChildTagsByTag.Find(CurrentTag))
		{
			for (const FGameplayTag& ChildTag : *ChildTags)
			{
				if (!GroupNodeSet.Contains(ChildTag))
				{
					continue;
				}

				int32& RemainingParentCount = RemainingParentCountByTag.FindOrAdd(ChildTag);
				RemainingParentCount = FMath::Max(0, RemainingParentCount - 1);
				if (RemainingParentCount <= 0)
				{
					ReadyTags.Add(ChildTag);
				}
			}
		}

		SortNodeTags(ReadyTags);
	}

	if (OutNodeOrder.Num() == GroupNodeTags.Num())
	{
		return;
	}

	TArray<FGameplayTag> RemainingTags;
	RemainingTags.Reserve(GroupNodeTags.Num() - OutNodeOrder.Num());
	for (const FGameplayTag& NodeTag : GroupNodeTags)
	{
		if (!ProcessedTags.Contains(NodeTag))
		{
			RemainingTags.Add(NodeTag);
		}
	}

	SortNodeTags(RemainingTags);
	OutNodeOrder.Append(RemainingTags);
}

int32 ResolvePreferredLocalColumn(
	const FGameplayTag& NodeTag,
	const TMap<FGameplayTag, TArray<FGameplayTag>>& ParentTagsByTag,
	const TSet<FGameplayTag>& GroupNodeSet,
	const TMap<FGameplayTag, int32>& AssignedColumnByTag,
	const TMap<FGameplayTag, int32>& RootSeedColumnByTag)
{
	int32 ColumnSum = 0;
	int32 ColumnCount = 0;
	if (const TArray<FGameplayTag>* ParentTags = ParentTagsByTag.Find(NodeTag))
	{
		for (const FGameplayTag& ParentTag : *ParentTags)
		{
			if (!GroupNodeSet.Contains(ParentTag))
			{
				continue;
			}

			if (const int32* ParentColumn = AssignedColumnByTag.Find(ParentTag))
			{
				ColumnSum += *ParentColumn;
				ColumnCount += 1;
			}
		}
	}

	if (ColumnCount > 0)
	{
		return FMath::RoundToInt(static_cast<float>(ColumnSum) / static_cast<float>(ColumnCount));
	}

	if (const int32* RootSeedColumn = RootSeedColumnByTag.Find(NodeTag))
	{
		return *RootSeedColumn;
	}

	return 0;
}

int32 FindBestFreeColumn(const TSet<int32>& UsedColumns, const int32 PreferredColumn)
{
	for (int32 Offset = 0; ; ++Offset)
	{
		if (Offset == 0)
		{
			if (!UsedColumns.Contains(PreferredColumn))
			{
				return PreferredColumn;
			}

			continue;
		}

		const int32 LeftColumn = PreferredColumn - Offset;
		const int32 RightColumn = PreferredColumn + Offset;
		const bool bLeftAvailable = !UsedColumns.Contains(LeftColumn);
		const bool bRightAvailable = !UsedColumns.Contains(RightColumn);

		if (bLeftAvailable && bRightAvailable)
		{
			const int32 LeftMagnitude = FMath::Abs(LeftColumn);
			const int32 RightMagnitude = FMath::Abs(RightColumn);
			if (LeftMagnitude != RightMagnitude)
			{
				return LeftMagnitude < RightMagnitude ? LeftColumn : RightColumn;
			}

			return LeftColumn < RightColumn ? LeftColumn : RightColumn;
		}

		if (bLeftAvailable)
		{
			return LeftColumn;
		}

		if (bRightAvailable)
		{
			return RightColumn;
		}
	}
}
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
	if (!SkillGraphCanvas || IsDesignTime())
	{
		ClearGraphWidgets();
		return;
	}

	TArray<UMSkillInstance*> SkillInstances;
	BuildSkillGraphModel(SkillInstances);

	if (SkillInstances.Num() <= 0)
	{
		ClearGraphWidgets();
		return;
	}

	const FVector2D GraphCanvasSize = ResolveGraphCanvasSize();
	TMap<FGameplayTag, int32> LayerByTag;
	TMap<FGameplayTag, int32> ColumnByTag;
	TMap<FGameplayTag, FVector2D> PositionByTag;
	LayoutSkillGraphNodes(SkillInstances, GraphCanvasSize, LayerByTag, ColumnByTag, PositionByTag);
	SynchronizeGraphWidgets(SkillInstances, LayerByTag, ColumnByTag, PositionByTag);
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

	Super::NativeDestruct();
}

int32 UMSkillTreeWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	for (const TPair<FGameplayTag, TObjectPtr<UMSkillTreeNodeWidget>>& Pair : NodeWidgetByTag)
	{
		if (!Pair.Key.IsValid() || !IsValid(Pair.Value))
		{
			continue;
		}

		UMSkillInstance* ChildSkillInstance = Pair.Value->GetSkillInstance();
		if (!ChildSkillInstance)
		{
			continue;
		}

		const FGeometry ChildGeometry = Pair.Value->GetCachedGeometry();
		const FVector2D ChildCenterLocal = AllottedGeometry.AbsoluteToLocal(ChildGeometry.GetAbsolutePosition() + (ChildGeometry.GetLocalSize() * 0.5f));

		for (const FGameplayTag& ParentTag : ChildSkillInstance->GetPrerequisiteSkillTags())
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

void UMSkillTreeWidget::BuildSkillGraphModel(TArray<UMSkillInstance*>& OutSkillInstances) const
{
	OutSkillInstances.Reset();

	if (!BoundPlayerCharacter)
	{
		return;
	}

	const UMPlayerSkillComponent* SkillComponent = BoundPlayerCharacter->GetSkillComponent();
	if (!SkillComponent || !SkillComponent->GetSkillInstances(OutSkillInstances))
	{
		return;
	}

	OutSkillInstances.RemoveAll([](const UMSkillInstance* SkillInstance)
	{
		return !SkillInstance || !SkillInstance->GetSkillTag().IsValid();
	});
}

void UMSkillTreeWidget::LayoutSkillGraphNodes(const TArray<UMSkillInstance*>& SkillInstances, const FVector2D& AvailableSize,
	TMap<FGameplayTag, int32>& OutLayerByTag, TMap<FGameplayTag, int32>& OutColumnByTag, TMap<FGameplayTag, FVector2D>& OutPositionByTag) const
{
	(void)AvailableSize;

	OutLayerByTag.Reset();
	OutColumnByTag.Reset();
	OutPositionByTag.Reset();

	TArray<FGameplayTag> NodeTags;
	TMap<FGameplayTag, TArray<FGameplayTag>> ParentTagsByTag;
	TMap<FGameplayTag, TArray<FGameplayTag>> ChildTagsByTag;
	TMap<FGameplayTag, int32> RequiredLevelByTag;
	TArray<int32> SortedLevels;
	TSet<FGameplayTag> ValidNodeTags;

	NodeTags.Reserve(SkillInstances.Num());
	ParentTagsByTag.Reserve(SkillInstances.Num());
	ChildTagsByTag.Reserve(SkillInstances.Num());
	RequiredLevelByTag.Reserve(SkillInstances.Num());

	for (UMSkillInstance* SkillInstance : SkillInstances)
	{
		if (!SkillInstance)
		{
			continue;
		}

		const FGameplayTag NodeTag = SkillInstance->GetSkillTag();
		if (!NodeTag.IsValid())
		{
			continue;
		}

		NodeTags.AddUnique(NodeTag);
		ParentTagsByTag.Add(NodeTag, SkillInstance->GetPrerequisiteSkillTags());
		ChildTagsByTag.FindOrAdd(NodeTag);

		const int32 RequiredLevel = SkillInstance->GetRequiredCharacterLevel();
		RequiredLevelByTag.Add(NodeTag, RequiredLevel);
		SortedLevels.AddUnique(RequiredLevel);
		ValidNodeTags.Add(NodeTag);
	}

	for (const FGameplayTag& NodeTag : NodeTags)
	{
		if (const TArray<FGameplayTag>* ParentTags = ParentTagsByTag.Find(NodeTag))
		{
			for (const FGameplayTag& ParentTag : *ParentTags)
			{
				if (ValidNodeTags.Contains(ParentTag))
				{
					ChildTagsByTag.FindOrAdd(ParentTag).AddUnique(NodeTag);
				}
			}
		}
	}

	SortedLevels.Sort();

	TMap<int32, int32> RowIndexByLevel;
	for (int32 RowIndex = 0; RowIndex < SortedLevels.Num(); ++RowIndex)
	{
		RowIndexByLevel.Add(SortedLevels[RowIndex], RowIndex);
	}

	TArray<FGameplayTag> SortedNodeTags = NodeTags;
	SortedNodeTags.Sort([](const FGameplayTag& A, const FGameplayTag& B)
	{
		return BuildStableNodeSortKey(A) < BuildStableNodeSortKey(B);
	});

	const bool bUseGridPanel = Cast<UGridPanel>(SkillGraphCanvas) != nullptr;
	const float StartX = CanvasPadding.X;
	const float StartY = CanvasPadding.Y;
	constexpr int32 RootSeedColumnSpacing = 2;
	constexpr int32 GroupColumnGap = 2;

	TArray<FNodeGroupLayoutInfo> GroupLayouts;
	GroupLayouts.Reserve(NodeTags.Num());

	TSet<FGameplayTag> VisitedNodeTags;
	VisitedNodeTags.Reserve(NodeTags.Num());

	TMap<FGameplayTag, int32> LocalColumnByTag;
	LocalColumnByTag.Reserve(NodeTags.Num());

	for (const FGameplayTag& StartTag : SortedNodeTags)
	{
		if (!StartTag.IsValid() || VisitedNodeTags.Contains(StartTag))
		{
			continue;
		}

		FNodeGroupLayoutInfo GroupLayout;
		GatherConnectedNodeGroup(StartTag, ValidNodeTags, ParentTagsByTag, ChildTagsByTag, VisitedNodeTags, GroupLayout.NodeTags);
		GroupLayout.NodeTags.Sort([](const FGameplayTag& A, const FGameplayTag& B)
		{
			return BuildStableNodeSortKey(A) < BuildStableNodeSortKey(B);
		});

		TSet<FGameplayTag> GroupNodeSet;
		GroupNodeSet.Reserve(GroupLayout.NodeTags.Num());

		for (const FGameplayTag& GroupNodeTag : GroupLayout.NodeTags)
		{
			GroupNodeSet.Add(GroupNodeTag);
			GroupLayout.MinRequiredLevel = FMath::Min(GroupLayout.MinRequiredLevel, RequiredLevelByTag.FindRef(GroupNodeTag));
		}

		for (const FGameplayTag& GroupNodeTag : GroupLayout.NodeTags)
		{
			bool bHasDisplayedParent = false;
			if (const TArray<FGameplayTag>* ParentTags = ParentTagsByTag.Find(GroupNodeTag))
			{
				for (const FGameplayTag& ParentTag : *ParentTags)
				{
					if (GroupNodeSet.Contains(ParentTag))
					{
						bHasDisplayedParent = true;
						break;
					}
				}
			}

			if (!bHasDisplayedParent)
			{
				GroupLayout.RootTags.Add(GroupNodeTag);
			}
		}

		GroupLayout.RootTags.Sort([&](const FGameplayTag& A, const FGameplayTag& B)
		{
			const int32 RequiredLevelA = RequiredLevelByTag.FindRef(A);
			const int32 RequiredLevelB = RequiredLevelByTag.FindRef(B);
			if (RequiredLevelA != RequiredLevelB)
			{
				return RequiredLevelA < RequiredLevelB;
			}

			return BuildStableNodeSortKey(A) < BuildStableNodeSortKey(B);
		});

		TMap<FGameplayTag, int32> RootSeedColumnByTag;
		RootSeedColumnByTag.Reserve(GroupLayout.RootTags.Num());

		// Leave room between root seeds so merge nodes can occupy the empty column between them.
		for (int32 RootIndex = 0; RootIndex < GroupLayout.RootTags.Num(); ++RootIndex)
		{
			RootSeedColumnByTag.Add(GroupLayout.RootTags[RootIndex], RootIndex * RootSeedColumnSpacing);
		}

		TArray<FGameplayTag> GroupNodeOrder;
		BuildGroupTopologicalOrder(GroupLayout.NodeTags, ParentTagsByTag, ChildTagsByTag, RequiredLevelByTag, RootSeedColumnByTag, GroupNodeOrder);

		TMap<int32, TSet<int32>> UsedColumnsByRow;
		GroupLayout.MinLocalColumn = MAX_int32;
		GroupLayout.MaxLocalColumn = MIN_int32;

		for (const FGameplayTag& GroupNodeTag : GroupNodeOrder)
		{
			const int32 RequiredLevel = RequiredLevelByTag.FindRef(GroupNodeTag);
			const int32 AssignedRow = RowIndexByLevel.FindRef(RequiredLevel);
			const int32 PreferredColumn = ResolvePreferredLocalColumn(GroupNodeTag, ParentTagsByTag, GroupNodeSet, LocalColumnByTag, RootSeedColumnByTag);
			int32 AssignedColumn = PreferredColumn;

			if (TSet<int32>* UsedColumns = UsedColumnsByRow.Find(AssignedRow))
			{
				AssignedColumn = FindBestFreeColumn(*UsedColumns, PreferredColumn);
			}

			UsedColumnsByRow.FindOrAdd(AssignedRow).Add(AssignedColumn);
			LocalColumnByTag.Add(GroupNodeTag, AssignedColumn);
			OutLayerByTag.Add(GroupNodeTag, AssignedRow);

			GroupLayout.MinLocalColumn = FMath::Min(GroupLayout.MinLocalColumn, AssignedColumn);
			GroupLayout.MaxLocalColumn = FMath::Max(GroupLayout.MaxLocalColumn, AssignedColumn);
		}

		if (GroupLayout.MinLocalColumn == MAX_int32 || GroupLayout.MaxLocalColumn == MIN_int32)
		{
			GroupLayout.MinLocalColumn = 0;
			GroupLayout.MaxLocalColumn = 0;
		}

		GroupLayouts.Add(MoveTemp(GroupLayout));
	}

	GroupLayouts.Sort([](const FNodeGroupLayoutInfo& A, const FNodeGroupLayoutInfo& B)
	{
		if (A.MinRequiredLevel != B.MinRequiredLevel)
		{
			return A.MinRequiredLevel < B.MinRequiredLevel;
		}

		const FString SortKeyA = A.RootTags.Num() > 0 ? BuildStableNodeSortKey(A.RootTags[0]) : (A.NodeTags.Num() > 0 ? BuildStableNodeSortKey(A.NodeTags[0]) : FString());
		const FString SortKeyB = B.RootTags.Num() > 0 ? BuildStableNodeSortKey(B.RootTags[0]) : (B.NodeTags.Num() > 0 ? BuildStableNodeSortKey(B.NodeTags[0]) : FString());
		if (SortKeyA != SortKeyB)
		{
			return SortKeyA < SortKeyB;
		}

		return A.NodeTags.Num() > B.NodeTags.Num();
	});

	int32 NextGroupStartColumn = 0;
	for (const FNodeGroupLayoutInfo& GroupLayout : GroupLayouts)
	{
		const int32 GroupColumnOffset = NextGroupStartColumn - GroupLayout.MinLocalColumn;
		for (const FGameplayTag& NodeTag : GroupLayout.NodeTags)
		{
			const int32 AssignedRow = OutLayerByTag.FindRef(NodeTag);
			const int32 AssignedColumn = GroupColumnOffset + LocalColumnByTag.FindRef(NodeTag);
			OutColumnByTag.Add(NodeTag, AssignedColumn);

			if (!bUseGridPanel)
			{
				const float PosX = StartX + (AssignedColumn * NodeHorizontalSpacing);
				const float PosY = StartY + (AssignedRow * NodeVerticalSpacing);
				OutPositionByTag.Add(NodeTag, FVector2D(PosX, PosY));
			}
			else
			{
				OutPositionByTag.Add(NodeTag, FVector2D::ZeroVector);
			}
		}

		NextGroupStartColumn += (GroupLayout.MaxLocalColumn - GroupLayout.MinLocalColumn + 1) + GroupColumnGap;
	}
}

void UMSkillTreeWidget::SynchronizeGraphWidgets(const TArray<UMSkillInstance*>& SkillInstances, const TMap<FGameplayTag, int32>& LayerByTag,
	const TMap<FGameplayTag, int32>& ColumnByTag, const TMap<FGameplayTag, FVector2D>& PositionByTag)
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

	TMap<FGameplayTag, UMSkillInstance*> SkillInstanceByTag;
	TSet<FGameplayTag> ValidNodeTags;
	ValidNodeTags.Reserve(SkillInstances.Num());
	SkillInstanceByTag.Reserve(SkillInstances.Num());

	for (UMSkillInstance* SkillInstance : SkillInstances)
	{
		if (!SkillInstance)
		{
			continue;
		}

		const FGameplayTag SkillTag = SkillInstance->GetSkillTag();
		if (SkillTag.IsValid())
		{
			ValidNodeTags.Add(SkillTag);
			SkillInstanceByTag.Add(SkillTag, SkillInstance);
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

	for (UMSkillInstance* SkillInstance : SkillInstances)
	{
		if (!SkillInstance)
		{
			continue;
		}

		const FGameplayTag SkillTag = SkillInstance->GetSkillTag();
		if (!SkillTag.IsValid())
		{
			continue;
		}

		UMSkillTreeNodeWidget* NodeWidget = NodeWidgetByTag.FindRef(SkillTag);
		if (!IsValid(NodeWidget))
		{
			NodeWidget = CreateWidget<UMSkillTreeNodeWidget>(GetOwningPlayer(), SkillTreeNodeWidgetClass);
		}

		if (!NodeWidget)
		{
			continue;
		}

		if (!NodeWidgetByTag.Contains(SkillTag))
		{
			SkillGraphCanvas->AddChild(NodeWidget);
			NodeWidgetByTag.Add(SkillTag, NodeWidget);
		}

		if (UMSkillInstance* ResolvedSkillInstance = SkillInstanceByTag.FindRef(SkillTag))
		{
			NodeWidget->SetSkillInstance(ResolvedSkillInstance);
		}

		const int32* Row = LayerByTag.Find(SkillTag);
		const int32* Column = ColumnByTag.Find(SkillTag);
		const FVector2D* Position = PositionByTag.Find(SkillTag);
		if (Row && Column && Position)
		{
			UpdateNodeWidgetLayout(NodeWidget, *Row, *Column, *Position);
		}
	}
}

void UMSkillTreeWidget::UpdateNodeWidgetLayout(UMSkillTreeNodeWidget* NodeWidget, const int32 Row, const int32 Column, const FVector2D& Position) const
{
	if (!NodeWidget)
	{
		return;
	}

	if (UGridSlot* GridSlot = Cast<UGridSlot>(NodeWidget->Slot))
	{
		GridSlot->SetRow(Row);
		GridSlot->SetColumn(Column);
		GridSlot->SetHorizontalAlignment(HAlign_Center);
		GridSlot->SetVerticalAlignment(VAlign_Center);
		return;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NodeWidget->Slot))
	{
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetPosition(Position);
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

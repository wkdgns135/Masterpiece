// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/MUIManagerSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/UI/MUIDeveloperSettings.h"
#include "Gameplay/UI/MUIGameplayTags.h"
#include "Gameplay/UI/MUIWidgetRegistryDataAsset.h"
#include "Gameplay/UI/Widget/MTaggedWidget.h"
#include "Gameplay/UI/Widget/MUIStackableWidget.h"

UMTaggedWidget* UMUIManagerSubsystem::CreateWidgetByTag(const FGameplayTag WidgetTag)
{
	return CreateWidgetInternal(ResolveWidgetClassByTag(WidgetTag));
}

UMTaggedWidget* UMUIManagerSubsystem::ShowWidgetByTag(const FGameplayTag WidgetTag)
{
	UMTaggedWidget* Widget = CreateWidgetByTag(WidgetTag);
	if (!Widget)
	{
		return nullptr;
	}

	if (IMUIStackableWidget* StackableWidget = Cast<IMUIStackableWidget>(Widget))
	{
		Widget->AddToViewport(StackableWidget->GetStackWidgetZOrder());
		StackableWidget->ActivateStackWidget();
		return Widget;
	}

	Widget->AddToViewport(MUIGameplayTags::GetWidgetBaseZOrder(Widget->GetWidgetTag()));
	return Widget;
}

UMTaggedWidget* UMUIManagerSubsystem::PushWidget(const TSubclassOf<UMTaggedWidget> WidgetClass)
{
	UMTaggedWidget* NewWidget = CreateWidgetInternal(WidgetClass);
	if (!NewWidget || !IsStackableWidget(NewWidget))
	{
		return nullptr;
	}

	FGameplayTag LayerTag;
	if (!ResolveLayerTagFromWidget(NewWidget, LayerTag))
	{
		return nullptr;
	}

	IMUIStackableWidget* StackableWidget = Cast<IMUIStackableWidget>(NewWidget);
	if (!StackableWidget)
	{
		return nullptr;
	}

	TArray<TWeakObjectPtr<UMTaggedWidget>>& Stack = LayerStacks.FindOrAdd(LayerTag);
	if (Stack.Num() > 0 && Stack.Last().IsValid())
	{
		if (IMUIStackableWidget* TopStackable = Cast<IMUIStackableWidget>(Stack.Last().Get()))
		{
			TopStackable->DeactivateStackWidget();
		}
	}

	NewWidget->AddToViewport(StackableWidget->GetStackWidgetZOrder());
	StackableWidget->ActivateStackWidget();
	Stack.Add(NewWidget);
	return NewWidget;
}

UMTaggedWidget* UMUIManagerSubsystem::PushWidgetByTag(const FGameplayTag WidgetTag)
{
	return PushWidget(ResolveWidgetClassByTag(WidgetTag));
}

bool UMUIManagerSubsystem::PopTopWidget(const FGameplayTag LayerTag)
{
	return PopTopWidgetInternal(LayerTag);
}

bool UMUIManagerSubsystem::HandleEscapeAction()
{
	const FGameplayTag EscapeOrder[] =
	{
		MUIGameplayTags::UI_Widget_Modal,
		MUIGameplayTags::UI_Widget_GameMenu,
		MUIGameplayTags::UI_Widget_GameHUD
	};

	for (const FGameplayTag& LayerTag : EscapeOrder)
	{
		UMTaggedWidget* TopWidget = GetTopWidget(LayerTag);
		IMUIStackableWidget* StackableWidget = TopWidget ? Cast<IMUIStackableWidget>(TopWidget) : nullptr;
		if (!StackableWidget || !StackableWidget->CanHandleStackEscape())
		{
			continue;
		}

		if (StackableWidget->HandleStackEscapeRequested())
		{
			return PopTopWidgetInternal(LayerTag);
		}
		return true;
	}

	return false;
}

bool UMUIManagerSubsystem::ToggleWidgetByTag(const FGameplayTag WidgetTag)
{
	TSubclassOf<UMTaggedWidget> WidgetClass = ResolveWidgetClassByTag(WidgetTag);
	if (!WidgetClass)
	{
		return false;
	}

	const UMTaggedWidget* WidgetCDO = WidgetClass->GetDefaultObject<UMTaggedWidget>();
	if (!WidgetCDO || !IsStackableWidget(WidgetCDO))
	{
		return false;
	}

	FGameplayTag LayerTag;
	if (!ResolveLayerTagFromWidget(WidgetCDO, LayerTag))
	{
		return false;
	}

	if (const UMTaggedWidget* TopWidget = GetTopWidget(LayerTag))
	{
		if (TopWidget->MatchesWidgetTag(WidgetTag))
		{
			return PopTopWidget(LayerTag);
		}
	}

	return PushWidget(WidgetClass) != nullptr;
}

UMTaggedWidget* UMUIManagerSubsystem::GetTopWidget(const FGameplayTag LayerTag) const
{
	const TArray<TWeakObjectPtr<UMTaggedWidget>>* Stack = LayerStacks.Find(LayerTag);
	if (!Stack || Stack->Num() <= 0)
	{
		return nullptr;
	}

	return Stack->Last().Get();
}

UMTaggedWidget* UMUIManagerSubsystem::GetTopWidgetByTag(const FGameplayTag WidgetTag) const
{
	for (const TPair<FGameplayTag, TArray<TWeakObjectPtr<UMTaggedWidget>>>& Pair : LayerStacks)
	{
		if (Pair.Value.Num() <= 0)
		{
			continue;
		}

		UMTaggedWidget* TopWidget = Pair.Value.Last().Get();
		if (TopWidget && TopWidget->MatchesWidgetTag(WidgetTag))
		{
			return TopWidget;
		}
	}

	return nullptr;
}

APlayerController* UMUIManagerSubsystem::ResolvePlayerController() const
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	return LocalPlayer ? LocalPlayer->GetPlayerController(GetWorld()) : nullptr;
}

UMTaggedWidget* UMUIManagerSubsystem::CreateWidgetInternal(const TSubclassOf<UMTaggedWidget> WidgetClass) const
{
	APlayerController* PlayerController = ResolvePlayerController();
	if (!PlayerController || !WidgetClass)
	{
		return nullptr;
	}

	return CreateWidget<UMTaggedWidget>(PlayerController, WidgetClass);
}

bool UMUIManagerSubsystem::ResolveLayerTagFromWidget(const UMTaggedWidget* Widget, FGameplayTag& OutLayerTag) const
{
	return Widget && MUIGameplayTags::TryResolveWidgetLayerTag(Widget->GetWidgetTag(), OutLayerTag);
}

bool UMUIManagerSubsystem::IsStackableWidget(const UMTaggedWidget* Widget) const
{
	return Widget && Widget->GetClass()->ImplementsInterface(UMUIStackableWidget::StaticClass());
}

bool UMUIManagerSubsystem::PopTopWidgetInternal(const FGameplayTag LayerTag)
{
	TArray<TWeakObjectPtr<UMTaggedWidget>>* Stack = LayerStacks.Find(LayerTag);
	if (!Stack || Stack->Num() <= 0)
	{
		return false;
	}

	TWeakObjectPtr<UMTaggedWidget> TopWidget = Stack->Last();
	Stack->Pop();

	if (TopWidget.IsValid())
	{
		if (IMUIStackableWidget* StackableWidget = Cast<IMUIStackableWidget>(TopWidget.Get()))
		{
			StackableWidget->DeactivateStackWidget();
		}

		TopWidget->RemoveFromParent();
	}

	if (Stack->Num() > 0 && Stack->Last().IsValid())
	{
		if (IMUIStackableWidget* StackableWidget = Cast<IMUIStackableWidget>(Stack->Last().Get()))
		{
			StackableWidget->ActivateStackWidget();
		}
	}

	return true;
}

const UMUIWidgetRegistryDataAsset* UMUIManagerSubsystem::GetWidgetRegistryForLayer(const FGameplayTag LayerTag)
{
	const UMUIDeveloperSettings* UISettings = GetDefault<UMUIDeveloperSettings>();
	if (!UISettings || !LayerTag.IsValid())
	{
		return nullptr;
	}

	if (const TObjectPtr<UMUIWidgetRegistryDataAsset>* LoadedRegistry = LoadedLayerRegistries.Find(LayerTag))
	{
		return LoadedRegistry->Get();
	}

	TSoftObjectPtr<UMUIWidgetRegistryDataAsset> RegistryAsset;
	if (!UISettings->FindWidgetRegistryAssetByLayerTag(LayerTag, RegistryAsset) || RegistryAsset.IsNull())
	{
		return nullptr;
	}

	UMUIWidgetRegistryDataAsset* LoadedRegistry = RegistryAsset.LoadSynchronous();
	if (!LoadedRegistry)
	{
		return nullptr;
	}

	LoadedLayerRegistries.Add(LayerTag, LoadedRegistry);
	return LoadedRegistry;
}

TSubclassOf<UMTaggedWidget> UMUIManagerSubsystem::ResolveWidgetClassByTag(const FGameplayTag WidgetTag) const
{
	FGameplayTag LayerTag;
	if (!MUIGameplayTags::TryResolveWidgetLayerTag(WidgetTag, LayerTag))
	{
		return nullptr;
	}

	UMUIManagerSubsystem* MutableThis = const_cast<UMUIManagerSubsystem*>(this);
	const UMUIWidgetRegistryDataAsset* WidgetRegistry = MutableThis->GetWidgetRegistryForLayer(LayerTag);
	return WidgetRegistry ? WidgetRegistry->FindWidgetClassByTag(WidgetTag) : nullptr;
}

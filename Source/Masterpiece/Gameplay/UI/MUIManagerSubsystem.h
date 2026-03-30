// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MUIManagerSubsystem.generated.h"

class UMTaggedWidget;
class UMUIWidgetRegistry;

UCLASS()
class MASTERPIECE_API UMUIManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI")
	UMTaggedWidget* CreateWidgetByTag(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable, Category="UI")
	UMTaggedWidget* ShowWidgetByTag(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable, Category="UI")
	UMTaggedWidget* PushWidget(TSubclassOf<UMTaggedWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category="UI")
	UMTaggedWidget* PushWidgetByTag(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable, Category="UI")
	bool PopTopWidget(FGameplayTag LayerTag);

	// Global back action path (UI.Action.Escape).
	UFUNCTION(BlueprintCallable, Category="UI")
	bool HandleEscapeAction();

	UFUNCTION(BlueprintCallable, Category="UI")
	bool ToggleWidgetByTag(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintPure, Category="UI")
	UMTaggedWidget* GetTopWidget(FGameplayTag LayerTag) const;

	UFUNCTION(BlueprintPure, Category="UI")
	UMTaggedWidget* GetTopWidgetByTag(FGameplayTag WidgetTag) const;

private:
	APlayerController* ResolvePlayerController() const;
	UMTaggedWidget* CreateWidgetInternal(TSubclassOf<UMTaggedWidget> WidgetClass) const;
	bool ResolveLayerTagFromWidget(const UMTaggedWidget* Widget, FGameplayTag& OutLayerTag) const;
	bool IsStackableWidget(const UMTaggedWidget* Widget) const;
	bool PopTopWidgetInternal(FGameplayTag LayerTag);
	const UMUIWidgetRegistry* GetWidgetRegistryForLayer(FGameplayTag LayerTag);
	TSubclassOf<UMTaggedWidget> ResolveWidgetClassByTag(FGameplayTag WidgetTag) const;

private:
	TMap<FGameplayTag, TArray<TWeakObjectPtr<UMTaggedWidget>>> LayerStacks;
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UMUIWidgetRegistry>> LoadedLayerRegistries;
};

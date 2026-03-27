// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UI/MUICommonTypes.h"
#include "Gameplay/UI/Widget/MTaggedWidget.h"
#include "Gameplay/UI/Widget/MUIStackableWidget.h"
#include "MActivatableWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMActivatableWidget : public UMTaggedWidget, public IMUIStackableWidget
{
	GENERATED_BODY()

public:
	UMActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="UI")
	void ActivateWidget();

	UFUNCTION(BlueprintCallable, Category="UI")
	void DeactivateWidget();

	UFUNCTION(BlueprintPure, Category="UI")
	bool CanHandleEscape() const;

	// Returns true if this widget consumed the Escape action.
	UFUNCTION(BlueprintNativeEvent, Category="UI")
	bool HandleEscapeRequested();
	virtual bool HandleEscapeRequested_Implementation();

	UFUNCTION(BlueprintPure, Category="UI")
	int32 GetDesiredZOrder() const;

	virtual void ActivateStackWidget() override;
	virtual void DeactivateStackWidget() override;
	virtual bool CanHandleStackEscape() const override;
	virtual bool HandleStackEscapeRequested() override;
	virtual int32 GetStackWidgetZOrder() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	EMUIInputPolicy InputPolicy = EMUIInputPolicy::GameAndMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	bool bCloseOnEscape = true;

	// Optional per-widget z-order delta over the layer base z-order.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	int32 ZOrderBias = 0;

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void K2_OnWidgetActivated();

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void K2_OnWidgetDeactivated();

private:
	void ApplyInputPolicy() const;
};

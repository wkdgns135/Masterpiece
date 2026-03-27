// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MUIStackableWidget.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UMUIStackableWidget : public UInterface
{
	GENERATED_BODY()
};

class MASTERPIECE_API IMUIStackableWidget
{
	GENERATED_BODY()

public:
	virtual void ActivateStackWidget() = 0;
	virtual void DeactivateStackWidget() = 0;
	virtual bool CanHandleStackEscape() const = 0;
	virtual bool HandleStackEscapeRequested() = 0;
	virtual int32 GetStackWidgetZOrder() const = 0;
};

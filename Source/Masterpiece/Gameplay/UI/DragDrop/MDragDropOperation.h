// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MDragDropOperation.generated.h"

class UMDefinitionInstance;
class UUserWidget;

UCLASS(BlueprintType)
class MASTERPIECE_API UMDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI|DragDrop")
	UMDefinitionInstance* GetDefinitionInstance() const;

	UFUNCTION(BlueprintPure, Category="UI|DragDrop")
	UUserWidget* GetSourceWidget() const;

public:
	UPROPERTY(BlueprintReadOnly, Category="UI|DragDrop")
	TObjectPtr<UMDefinitionInstance> DefinitionInstance;

	UPROPERTY(BlueprintReadOnly, Category="UI|DragDrop")
	TObjectPtr<UUserWidget> SourceWidget;
};

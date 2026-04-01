// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/UI/DragDrop/MDragDropOperation.h"

#include "Blueprint/UserWidget.h"

UMDefinitionInstance* UMDragDropOperation::GetDefinitionInstance() const
{
	return DefinitionInstance.Get();
}

UUserWidget* UMDragDropOperation::GetSourceWidget() const
{
	return SourceWidget.Get();
}

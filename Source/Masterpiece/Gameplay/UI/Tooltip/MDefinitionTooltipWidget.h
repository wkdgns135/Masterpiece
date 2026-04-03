#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MDefinitionTooltipWidget.generated.h"

class UMDefinitionInstance;

UCLASS(Abstract, Blueprintable)
class MASTERPIECE_API UMDefinitionTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI|Tooltip")
	void SetDefinitionInstance(UMDefinitionInstance* InDefinitionInstance);

	UFUNCTION(BlueprintPure, Category="UI|Tooltip")
	UMDefinitionInstance* GetDefinitionInstance() const;

protected:
	virtual void NativeConstruct() override;
	virtual void HandleDefinitionInstanceSet();

	UFUNCTION(BlueprintImplementableEvent, Category="UI|Tooltip")
	void K2_OnDefinitionInstanceSet(UMDefinitionInstance* InDefinitionInstance);

private:
	UPROPERTY(Transient)
	TObjectPtr<UMDefinitionInstance> DefinitionInstance;
};

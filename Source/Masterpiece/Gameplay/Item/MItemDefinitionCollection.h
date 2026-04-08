#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Definition/MDefinitionCollection.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "MItemDefinitionCollection.generated.h"

class UMDefinitionObject;
class UMItemDefinition;

UCLASS(BlueprintType)
class MASTERPIECE_API UMItemDefinitionCollection : public UMDefinitionCollection
{
	GENERATED_BODY()

public:
	const TArray<TObjectPtr<UMItemDefinition>>& GetItemDefinitions() const;
	UMItemDefinition* FindItemDefinitionByTag(FGameplayTag ItemTag) const;
	virtual void GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

private:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category="ItemDefinitionCollection", meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UMItemDefinition>> ItemDefinitions;
};

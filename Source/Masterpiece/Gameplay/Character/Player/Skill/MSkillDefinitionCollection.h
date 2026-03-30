#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Definition/MDefinitionCollection.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "MSkillDefinitionCollection.generated.h"

class UMDefinitionObject;
class UMSkillDefinition;

UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillDefinitionCollection : public UMDefinitionCollection
{
	GENERATED_BODY()

public:
	const TArray<TObjectPtr<UMSkillDefinition>>& GetSkillDefinitions() const;
	UMSkillDefinition* FindSkillDefinitionByTag(FGameplayTag SkillTag) const;
	virtual void GetDefinitions(TArray<UMDefinitionObject*>& OutDefinitions) const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

private:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category="SkillDefinitionCollection", meta=(AllowPrivateAccess="true", ToolTip="이 컬렉션에 포함될 스킬 정의 목록입니다. 각 항목은 인라인으로 추가하거나 제거할 수 있습니다."))
	TArray<TObjectPtr<UMSkillDefinition>> SkillDefinitions;
};

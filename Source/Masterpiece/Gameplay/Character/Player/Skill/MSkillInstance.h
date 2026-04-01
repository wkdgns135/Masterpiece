#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/Player/Skill/MSkillDefinition.h"
#include "Gameplay/Definition/MDefinitionInstance.h"
#include "MSkillInstance.generated.h"

class UTexture2D;

UCLASS(BlueprintType)
class MASTERPIECE_API UMSkillInstance : public UMDefinitionInstance
{
	GENERATED_BODY()

public:
	bool InitializeSkillInstance(UMSkillDefinition* InDefinition);

	UFUNCTION(BlueprintPure, Category="Skill")
	UMSkillDefinition* GetSkillDefinition() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	UMSkillDefinitionActive* GetActiveDefinition() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	UMSkillDefinitionPassive* GetPassiveDefinition() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	FGameplayTag GetSkillTag() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	TSoftObjectPtr<UTexture2D> GetIcon() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	FText GetDisplayName() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	FText GetDescription() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	TArray<FGameplayTag> GetPrerequisiteSkillTags() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	TMap<FGameplayTag, int32> GetPrerequisiteSkillRanks() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	int32 GetRequiredCharacterLevel() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	int32 GetCostPerRank() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	int32 GetMaxRank() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	bool IsPassive() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	bool IsUnlocked() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	bool IsEquipped() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	int32 GetCurrentRank() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	FGameplayTag GetAssignedSlotTag() const;

	void SetUnlocked(bool bInUnlocked);
	void SetCurrentRank(int32 InCurrentRank);
	void SetAssignedSlotTag(FGameplayTag InAssignedSlotTag);

public:
	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FGameplayTag SkillTag;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 RequiredCharacterLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 CostPerRank = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 MaxRank = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	TArray<FGameplayTag> ParentSkillTags;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	bool bPassive = false;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	bool bUnlocked = true;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	bool bEquipped = false;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	int32 CurrentRank = 1;

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FGameplayTag AssignedSlotTag;

private:
	const UMSkillDefinition* GetSkillDefinitionChecked() const;
	void RefreshSkillViewData();
};

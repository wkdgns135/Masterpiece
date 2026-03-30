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

	UMSkillDefinition* GetSkillDefinition() const;
	UMSkillDefinitionActive* GetActiveDefinition() const;
	UMSkillDefinitionPassive* GetPassiveDefinition() const;

	UFUNCTION(BlueprintPure, Category="Skill")
	FGameplayTag GetSkillTag() const;

	const TSoftObjectPtr<UTexture2D>& GetIcon() const;
	const FText& GetDisplayName() const;
	const FText& GetDescription() const;
	TArray<FGameplayTag> GetPrerequisiteSkillTags() const;
	const TMap<FGameplayTag, int32>& GetPrerequisiteSkillRanks() const;

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

private:
	const UMSkillDefinition* GetSkillDefinitionChecked() const;

private:
	UPROPERTY(Transient)
	bool bUnlocked = true;

	UPROPERTY(Transient)
	int32 CurrentRank = 1;

	UPROPERTY(Transient)
	FGameplayTag AssignedSlotTag;
};

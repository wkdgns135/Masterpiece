#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Character/Player/Skill/MSkillInstance.h"
#include "MPlayerSkillComponent.generated.h"

class UAbilitySystemComponent;
class UMDefinitionCollection;
class UMDefinitionSubsystem;
class UMSkillDefinition;
class UMSkillDefinitionActive;
class UMSkillDefinitionCollection;

DECLARE_MULTICAST_DELEGATE(FOnPlayerSkillLoadoutChanged);
DECLARE_MULTICAST_DELEGATE(FOnPlayerSkillStateChanged);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerSkillSlotChanged, FGameplayTag, FGameplayTag);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMPlayerSkillComponent();

	FOnPlayerSkillLoadoutChanged OnSkillLoadoutChanged;
	FOnPlayerSkillStateChanged OnSkillStateChanged;
	FOnPlayerSkillSlotChanged OnSkillSlotChanged;

	UFUNCTION(BlueprintPure, Category="Skill")
	bool GetSkillInstances(TArray<UMSkillInstance*>& OutSkillInstances) const;

	UFUNCTION(BlueprintPure, Category="Skill")
	UMSkillInstance* GetSkillInstance(const FGameplayTag& SkillTag) const;

	UFUNCTION(BlueprintPure, Category="Skill")
	FGameplayTag GetAssignedSkillTag(const FGameplayTag& SlotTag) const;

	UFUNCTION(BlueprintCallable, Category="Skill")
	bool EquipSkillToSlot(const FGameplayTag& SkillTag, const FGameplayTag& SlotTag, int32 SkillRank = 1);

	UFUNCTION(BlueprintCallable, Category="Skill")
	bool UnequipSkillSlot(const FGameplayTag& SlotTag);

	UFUNCTION(BlueprintPure, Category="Skill")
	bool IsSkillSlotEquipped(const FGameplayTag& SlotTag) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool EnsureSkillStateInitialized() const;
	void InitializeSkillState();
	void HandleSkillDefinitionCollectionLoaded(UMDefinitionCollection* LoadedCollection);
	void BroadcastSkillStateChanged(const TArray<FGameplayTag>& ChangedSlotTags);
	UAbilitySystemComponent* ResolveAbilitySystemComponent() const;
	UMDefinitionSubsystem* ResolveDefinitionSubsystem() const;
	const UMSkillDefinitionCollection* ResolveSkillDefinitionCollection() const;
	const UMSkillDefinition* ResolveSkillDefinition(const FGameplayTag& SkillTag) const;
	bool ClearEquippedSlotInternal(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& SlotTag, FGameplayTag* OutRemovedSkillTag = nullptr);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill", meta=(AllowPrivateAccess="true"))
	FGameplayTag SkillDefinitionCollectionTag;

	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UMSkillInstance>> SkillInstances;

	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayTag> EquippedSkillTagsBySlot;

	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> EquippedSkillHandlesBySlot;

	UPROPERTY(Transient)
	TObjectPtr<UMSkillDefinitionCollection> LoadedSkillDefinitionCollection;

	UPROPERTY(Transient)
	bool bSkillStateLoading = false;

	UPROPERTY(Transient)
	bool bSkillStateInitialized = false;
};

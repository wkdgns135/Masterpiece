// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MCombatComponent.generated.h"

class AMCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MASTERPIECE_API UMCombatComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMCombatComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintPure, Category="Combat")
	AMCharacterBase* GetOwnerCharacter() const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsSkillOnCooldown(FName SkillId, float CooldownSeconds) const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void CommitSkillUsage(FName SkillId);

private:
	UPROPERTY(Transient)
	TObjectPtr<AMCharacterBase> OwnerCharacter;

	UPROPERTY(Transient)
	TMap<FName, float> LastSkillUseTime;
};

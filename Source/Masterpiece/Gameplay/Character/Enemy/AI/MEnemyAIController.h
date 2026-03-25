// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MEnemyAIController.generated.h"

class AMEnemyCharacterBase;
class UBehaviorTree;

UCLASS()
class MASTERPIECE_API AMEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AMEnemyCharacterBase> ControlledEnemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAIPerceptionComponent> AIPerception;
};

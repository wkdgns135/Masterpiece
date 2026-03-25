// Fill out your copyright notice in the Description page of Project Settings.

#include "MEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/Character/Enemy/MEnemyCharacterBase.h"
#include "Perception/AIPerceptionComponent.h"

AMEnemyAIController::AMEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AMEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<AMEnemyCharacterBase>(InPawn);
	if (!ControlledEnemy || !BehaviorTreeAsset)
	{
		return;
	}
	
	UBlackboardComponent* BBComp = nullptr;
	if (BehaviorTreeAsset && UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BBComp))
	{
		BBComp->SetValueAsVector(TEXT("HomeLocation"), ControlledEnemy->GetActorLocation());
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AMEnemyAIController::OnUnPossess()
{
	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("UnPossess"));
	}

	StopMovement();
	ControlledEnemy = nullptr;

	Super::OnUnPossess();
}

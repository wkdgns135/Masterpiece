#include "Gameplay/Animation/NotifyState/MAnimNotifyState_AttackTraceWindow.h"

#include "GameFramework/Actor.h"
#include "Gameplay/Character/Interface/MAttacker.h"

namespace
{
void ExecuteAttackTrace(USkeletalMeshComponent* MeshComp, const FName DamageSourceBone)
{
	if (!MeshComp)
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	if (IMAttacker* Attacker = Cast<IMAttacker>(OwnerActor))
	{
		Attacker->DoAttackTrace(DamageSourceBone);
	}
}
}

UMAnimNotifyState_AttackTraceWindow::UMAnimNotifyState_AttackTraceWindow()
{
}

FString UMAnimNotifyState_AttackTraceWindow::GetNotifyName_Implementation() const
{
	return DamageSourceBone.IsNone()
		? TEXT("M Attack Trace Window")
		: FString::Printf(TEXT("M Attack Trace [%s]"), *DamageSourceBone.ToString());
}

void UMAnimNotifyState_AttackTraceWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (bTraceOnBegin)
	{
		ExecuteAttackTrace(MeshComp, DamageSourceBone);
	}
}

void UMAnimNotifyState_AttackTraceWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (bTraceOnTick)
	{
		ExecuteAttackTrace(MeshComp, DamageSourceBone);
	}
}

void UMAnimNotifyState_AttackTraceWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (bTraceOnEnd)
	{
		ExecuteAttackTrace(MeshComp, DamageSourceBone);
	}
}

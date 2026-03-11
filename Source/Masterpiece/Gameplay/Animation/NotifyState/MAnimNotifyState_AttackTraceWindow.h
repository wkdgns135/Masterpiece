#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MAnimNotifyState_AttackTraceWindow.generated.h"

UCLASS(meta=(DisplayName="M Attack Trace Window"))
class MASTERPIECE_API UMAnimNotifyState_AttackTraceWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UMAnimNotifyState_AttackTraceWindow();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack Trace")
	FName DamageSourceBone = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack Trace")
	bool bTraceOnBegin = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack Trace")
	bool bTraceOnTick = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack Trace")
	bool bTraceOnEnd = false;
};

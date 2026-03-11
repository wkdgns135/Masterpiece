#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "MAnimNotifyState_SendGameplayEventWindow.generated.h"

UCLASS(meta=(DisplayName="M Gameplay Event Window"))
class MASTERPIECE_API UMAnimNotifyState_SendGameplayEventWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UMAnimNotifyState_SendGameplayEventWindow();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	FGameplayTag BeginEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	FGameplayTag EndEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	float EventMagnitude = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	bool bUseOwnerAsTarget = true;
};

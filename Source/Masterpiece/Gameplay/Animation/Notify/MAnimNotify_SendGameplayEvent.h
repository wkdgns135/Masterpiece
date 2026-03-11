#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "MAnimNotify_SendGameplayEvent.generated.h"

UCLASS(meta=(DisplayName="M Send Gameplay Event"))
class MASTERPIECE_API UMAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UMAnimNotify_SendGameplayEvent();

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	float EventMagnitude = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Event")
	bool bUseOwnerAsTarget = true;
};

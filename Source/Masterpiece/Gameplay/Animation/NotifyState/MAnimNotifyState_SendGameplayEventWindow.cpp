#include "Gameplay/Animation/NotifyState/MAnimNotifyState_SendGameplayEventWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"

namespace
{
void SendGameplayEventFromNotifyState(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FGameplayTag& EventTag,
	const float EventMagnitude, const bool bUseOwnerAsTarget)
{
	if (!MeshComp || !EventTag.IsValid())
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = OwnerActor;
	Payload.Target = bUseOwnerAsTarget ? OwnerActor : nullptr;
	Payload.OptionalObject = Animation;
	Payload.ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
	Payload.EventMagnitude = EventMagnitude;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, EventTag, Payload);
}
}

UMAnimNotifyState_SendGameplayEventWindow::UMAnimNotifyState_SendGameplayEventWindow()
{
}

FString UMAnimNotifyState_SendGameplayEventWindow::GetNotifyName_Implementation() const
{
	if (BeginEventTag.IsValid() && EndEventTag.IsValid())
	{
		return FString::Printf(TEXT("%s -> %s"), *BeginEventTag.ToString(), *EndEventTag.ToString());
	}

	if (BeginEventTag.IsValid())
	{
		return BeginEventTag.ToString();
	}

	if (EndEventTag.IsValid())
	{
		return EndEventTag.ToString();
	}

	return TEXT("M Gameplay Event Window");
}

void UMAnimNotifyState_SendGameplayEventWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	SendGameplayEventFromNotifyState(MeshComp, Animation, BeginEventTag, EventMagnitude, bUseOwnerAsTarget);
}

void UMAnimNotifyState_SendGameplayEventWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	SendGameplayEventFromNotifyState(MeshComp, Animation, EndEventTag, EventMagnitude, bUseOwnerAsTarget);
}

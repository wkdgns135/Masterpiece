#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MGameplayCheatManager.generated.h"

UCLASS()
class MASTERPIECE_API UMGameplayCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec, Category="Cheat")
	void AddItemByTag(FString ItemTagString, int32 Quantity = 1);

private:
	void SendCheatMessage(const FString& Message) const;
};

#include "Gameplay/Test/Cheat/MGameplayCheatManager.h"

#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerInventoryComponent.h"

void UMGameplayCheatManager::AddItemByTag(FString ItemTagString, const int32 Quantity)
{
	const FString TrimmedTagString = ItemTagString.TrimStartAndEnd();
	if (TrimmedTagString.IsEmpty())
	{
		SendCheatMessage(TEXT("Usage: AddItemByTag Item.Tag [Quantity]"));
		return;
	}

	if (Quantity <= 0)
	{
		SendCheatMessage(TEXT("Quantity must be greater than 0."));
		return;
	}

	const FGameplayTag ItemTag = FGameplayTag::RequestGameplayTag(FName(*TrimmedTagString), false);
	if (!ItemTag.IsValid())
	{
		SendCheatMessage(FString::Printf(TEXT("Invalid item tag: %s"), *TrimmedTagString));
		return;
	}

	APlayerController* PlayerController = GetTypedOuter<APlayerController>();
	AMPlayerCharacterBase* PlayerCharacter = PlayerController ? Cast<AMPlayerCharacterBase>(PlayerController->GetPawn()) : nullptr;
	UMPlayerInventoryComponent* InventoryComponent = PlayerCharacter ? PlayerCharacter->GetInventoryComponent() : nullptr;
	if (!InventoryComponent)
	{
		SendCheatMessage(TEXT("Player inventory component was not found."));
		return;
	}

	if (!InventoryComponent->AddItemByTag(ItemTag, Quantity))
	{
		SendCheatMessage(FString::Printf(TEXT("Failed to add %d x %s."), Quantity, *ItemTag.ToString()));
		return;
	}

	SendCheatMessage(FString::Printf(TEXT("Added %d x %s to inventory."), Quantity, *ItemTag.ToString()));
}

void UMGameplayCheatManager::SendCheatMessage(const FString& Message) const
{
	if (Message.IsEmpty())
	{
		return;
	}

	if (APlayerController* PlayerController = GetTypedOuter<APlayerController>())
	{
		PlayerController->ClientMessage(Message);
	}
}

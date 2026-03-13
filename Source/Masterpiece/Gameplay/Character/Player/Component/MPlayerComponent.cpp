// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "Gameplay/MGameplayTags.h"
#include "Gameplay/Character/Player/MPlayerCharacterBase.h"
#include "Gameplay/Character/Player/Component/MPlayerCameraComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerCombatComponent.h"
#include "Gameplay/Character/Player/Input/MPlayerInputComponent.h"
#include "Gameplay/Character/Player/Component/MPlayerMovementComponent.h"
#include "Gameplay/Character/Player/Input/MPlayerInputConfig.h"
#include "Gameplay/Interface/MDamageable.h"
#include "Gameplay/PlayerController/MGameplayPlayerController.h"

UMPlayerComponent::UMPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMPlayerComponent::IsPlayerComponentInitialized() const
{
	return bPlayerComponentInitialized;
}

void UMPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	const AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	if (!ensureMsgf(PlayerCharacter, TEXT("UMPlayerComponent owner must be AMPlayerCharacterBase. Component: %s"), *GetName()))
	{
		return;
	}

	bPlayerComponentInitialized = true;
	BindInputActions();
}

void UMPlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bPlayerComponentInitialized)
	{
		UnbindInputActions();
		bPlayerComponentInitialized = false;
	}

	Super::EndPlay(EndPlayReason);
}

void UMPlayerComponent::HandleRightClickCommand(const FInputActionValue& Value)
{
	AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter();
	AMGameplayPlayerController* PlayerController = GetMGameplayPlayerController();
	if (!PlayerCharacter || !PlayerController)
	{
		return;
	}

	FHitResult CursorHit;
	if (!PlayerController->TraceCursorToWorld(CursorHit))
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = MGameplayTags::Event_Move_Request;
	Payload.Instigator = PlayerCharacter;
	Payload.ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
	Payload.ContextHandle.AddHitResult(CursorHit);

	AActor* HitActor = CursorHit.GetActor();
	if (IsValid(HitActor) && HitActor->GetClass()->ImplementsInterface(UMDamageable::StaticClass()))
	{
		Payload.EventTag = MGameplayTags::Event_Attack_Request;
		Payload.Target = HitActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, MGameplayTags::Event_Attack_Request, Payload);
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, MGameplayTags::Event_Move_Request, Payload);
}

void UMPlayerComponent::HandleCursorAim(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter())
	{
		if (UMPlayerMovementComponent* MovementComponent = PlayerCharacter->GetPlayerMovementComponent())
		{
			MovementComponent->FaceCursorDirection();
		}
	}
}

void UMPlayerComponent::HandleZoom(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter())
	{
		if (UMPlayerCameraComponent* CameraComponent = PlayerCharacter->GetPlayerCameraComponent())
		{
			CameraComponent->HandleZoomInput(Value);
		}
	}
}

void UMPlayerComponent::HandleDodge()
{
	if (AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter())
	{
		if (UMPlayerCombatComponent* CombatComponent = PlayerCharacter->GetPlayerCombatComponent())
		{
			CombatComponent->ExecuteDodge();
		}
	}
}

void UMPlayerComponent::HandleInteraction()
{
	if (AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter())
	{
		if (UMPlayerCombatComponent* CombatComponent = PlayerCharacter->GetPlayerCombatComponent())
		{
			CombatComponent->ExecuteInteraction();
		}
	}
}

void UMPlayerComponent::HandleSkillSlot(const FInputActionValue& Value)
{
	if (AMPlayerCharacterBase* PlayerCharacter = GetMPlayerCharacter())
	{
		if (UMPlayerCombatComponent* CombatComponent = PlayerCharacter->GetPlayerCombatComponent())
		{
			const int32 SkillSlotIndex = UMPlayerInputComponent::ResolveSkillSlotIndexFromInput(Value);
			CombatComponent->ExecuteSkillSlot(SkillSlotIndex);
		}
	}
}

void UMPlayerComponent::BindInputActions()
{
	UMPlayerInputComponent* InputComponent = GetMPlayerInputComponent();
	const UMPlayerInputConfig* InputConfig = InputComponent ? InputComponent->GetInputConfig() : nullptr;
	if (!InputComponent || !InputConfig)
	{
		return;
	}

	if (InputConfig->MoveCommandAction)
	{
		MoveCommandBindingHandle = InputComponent->BindAction(InputConfig->MoveCommandAction, ETriggerEvent::Started, this, &ThisClass::HandleRightClickCommand).GetHandle();
	}
	if (InputConfig->CursorAimAction)
	{
		CursorAimBindingHandle = InputComponent->BindAction(InputConfig->CursorAimAction, ETriggerEvent::Triggered, this, &ThisClass::HandleCursorAim).GetHandle();
	}
	if (InputConfig->ZoomAction)
	{
		ZoomBindingHandle = InputComponent->BindAction(InputConfig->ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::HandleZoom).GetHandle();
	}
	if (InputConfig->DodgeAction)
	{
		DodgeBindingHandle = InputComponent->BindAction(InputConfig->DodgeAction, ETriggerEvent::Started, this, &ThisClass::HandleDodge).GetHandle();
	}
	if (InputConfig->InteractionAction)
	{
		InteractionBindingHandle = InputComponent->BindAction(InputConfig->InteractionAction, ETriggerEvent::Started, this, &ThisClass::HandleInteraction).GetHandle();
	}
	if (InputConfig->SkillSlotAction)
	{
		SkillSlotBindingHandle = InputComponent->BindAction(InputConfig->SkillSlotAction, ETriggerEvent::Started, this, &ThisClass::HandleSkillSlot).GetHandle();
	}
}

void UMPlayerComponent::UnbindInputActions()
{
	UMPlayerInputComponent* InputComponent = GetMPlayerInputComponent();
	if (!InputComponent)
	{
		return;
	}

	if (MoveCommandBindingHandle != 0)
	{
		InputComponent->RemoveBindingByHandle(MoveCommandBindingHandle);
		MoveCommandBindingHandle = 0;
	}
	if (CursorAimBindingHandle != 0)
	{
		InputComponent->RemoveBindingByHandle(CursorAimBindingHandle);
		CursorAimBindingHandle = 0;
	}
	if (ZoomBindingHandle != 0)
	{
		InputComponent->RemoveBindingByHandle(ZoomBindingHandle);
		ZoomBindingHandle = 0;
	}
	if (DodgeBindingHandle != 0)
	{
		InputComponent->RemoveBindingByHandle(DodgeBindingHandle);
		DodgeBindingHandle = 0;
	}
	if (InteractionBindingHandle != 0)
	{
		InputComponent->RemoveBindingByHandle(InteractionBindingHandle);
		InteractionBindingHandle = 0;
	}
	if (SkillSlotBindingHandle != 0)
	{
		InputComponent->RemoveBindingByHandle(SkillSlotBindingHandle);
		SkillSlotBindingHandle = 0;
	}
}

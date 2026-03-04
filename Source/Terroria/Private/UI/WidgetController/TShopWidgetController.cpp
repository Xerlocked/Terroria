// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TShopWidgetController.h"

#include "AbilitySystem/Data/TAbilityDataAsset.h"
#include "Interface/PlayerInterface.h"
#include "Player/TPlayerController.h"
#include "Player/TPlayerState.h"
#include "Player/TShopComponent.h"

void UTShopWidgetController::BroadcastInitialValues()
{
	const ATPlayerController* PC = Cast<ATPlayerController>(PlayerController);
	PlayerShopComponent = PC ? PC->GetShopComponent() : nullptr;
	if (PlayerShopComponent)
	{
		// Broadcast initial shop item list to UI
		OnShopItemsUpdated.Broadcast(PlayerShopComponent->GetAllShopItems());
	}
	// Broadcast initial gold
	if (APawn* Pawn = PlayerController->GetPawn())
	{
		if (Pawn->Implements<UPlayerInterface>())
		{
			OnGoldChanged.Broadcast(IPlayerInterface::Execute_GetCurrentGold(Pawn));
		}
	}
}

void UTShopWidgetController::BindCallbacksToDependencies()
{
	ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);

	TPlayerState->OnGoldChangedDelegate.AddLambda
	([this](int32 InGold)
		{
			OnGoldChanged.Broadcast(InGold);
		}
	);

	if (UTAbilitySystemComponent* ASC = Cast<UTAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AbilityLevelChangedEvent.AddLambda(
			[this](const FGameplayTag& AbilityTag, int32 NewLevel)
			{
				OnAbilityLevelChanged.Broadcast(AbilityTag, NewLevel);
			}
		);
	}
}

void UTShopWidgetController::RequestPurchaseItem(const FGameplayTag& ItemTag)
{
	const bool bSuccess = PlayerShopComponent->PurchaseItem(PlayerController->GetPawn(), ItemTag);

	// Notify the UI of the result
	OnPurchaseResult.Broadcast(ItemTag, bSuccess);
	if (bSuccess)
	{
		// Notify purchase count update
		OnShopItemPurchased.Broadcast(PlayerShopComponent->GetShopItemContext(ItemTag));
	}
}

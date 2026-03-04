// Copyright 2025 @xerlock. All Rights Reserved.


#include "Player/TShopComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Interface/PlayerInterface.h"

UTShopComponent::UTShopComponent()
{
}

bool UTShopComponent::PurchaseItem(AActor* Buyer, const FShopItemTableRow& ItemData)
{
	if (!IsValid(Buyer) || !Buyer->Implements<UPlayerInterface>())
	{
		return false;
	}

	const int32 CurrentPurchaseLevel = GetPurchaseCount(ItemData.EventTag);
	const int32 Price = CalculatePrice(ItemData, CurrentPurchaseLevel);

	if (IPlayerInterface::Execute_GetCurrentGold(Buyer) < Price)
	{
		return false;
	}

	if (CurrentPurchaseLevel >= ItemData.MaxLevel)
	{
		return false;
	}

	if (IPlayerInterface::Execute_SpendGold(Buyer, Price))
	{
		PurchaseCountMap.FindOrAdd(ItemData.EventTag)++;

		const FGameplayTag AbilityCategoryTag = FGameplayTag::RequestGameplayTag(FName("Shop.Category.Ability"));

		if (ItemData.CategoryTag.MatchesTagExact(AbilityCategoryTag))
		{
			// 스킬인 경우: 데이터 애셋을 거치지 않고 태그를 이용해 바로 레벨업을 지시합니다.
			IPlayerInterface::Execute_UpgradeAbilityByTag(Buyer, ItemData.EventTag);
		}
		else
		{
			FGameplayEventData Payload;
			Payload.EventTag = ItemData.EventTag;
			Payload.EventMagnitude = ItemData.Magnitude;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Buyer, ItemData.EventTag, Payload);
		}


		return true;
	}

	return false;
}

bool UTShopComponent::PurchaseItem(AActor* Buyer, const FGameplayTag& ItemEventTag)
{
	for (const FShopItemTableRow& ItemData : GetAllShopItems())
	{
		if (ItemData.EventTag == ItemEventTag)
		{
			return PurchaseItem(Buyer, ItemData);
		}
	}

	return false;
}

TArray<FShopItemTableRow> UTShopComponent::GetAllShopItems() const
{
	TArray<FShopItemTableRow> AllItems;

	if (ShopDataTable)
	{
		static const FString ContextString(TEXT("ShopItemContext"));
		TArray<FShopItemTableRow*> Rows;
		ShopDataTable->GetAllRows(ContextString, Rows);

		for (const FShopItemTableRow* Row : Rows)
		{
			AllItems.Add(*Row);
		}
	}

	return AllItems;
}

FShopItemContext UTShopComponent::GetShopItemContext(const FGameplayTag& ItemEventTag) const
{
	FShopItemContext Context;

	for (const FShopItemTableRow& ItemData : GetAllShopItems())
	{
		if (ItemData.EventTag == ItemEventTag)
		{
			const int32 CurrentPurchaseLevel = GetPurchaseCount(ItemData.EventTag);
			const int32 Price = CalculatePrice(ItemData, CurrentPurchaseLevel);
			const bool bMaxLevelReached = CurrentPurchaseLevel >= ItemData.MaxLevel;

			Context.EventTag = ItemData.EventTag;
			Context.CurrentPurchaseCount = CurrentPurchaseLevel;
			Context.CurrentPrice = Price;
			Context.bIsMaxLevel = bMaxLevelReached;
		}
	}

	return Context;
}

int32 UTShopComponent::GetPurchaseCount(const FGameplayTag& EventTag) const
{
	const int32* Count = PurchaseCountMap.Find(EventTag);
	return Count ? *Count : 0;
}

int32 UTShopComponent::CalculatePrice(const FShopItemTableRow& ItemData, const int32 CurrentLevel) const
{
	float Price = 1.f;

	if (ItemData.PriceCurve.IsValid())
	{
		int32 TargetLevel = FMath::Clamp(CurrentLevel + 1, 1, ItemData.MaxLevel);

		Price = ItemData.PriceCurve.GetValueAtLevel(TargetLevel);
	}

	return Price;
}

// Copyright 2025 @xerlock. All Rights Reserved.


#include "Data/TShopDataAsset.h"

FAttributeShopInfo UTShopDataAsset::FindAttributeShopInfoByTag(const FGameplayTag& AttributeTag)
{
	for (const FAttributeShopInfo& AttributeInfo : AttributeShopInfo)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}

	return FAttributeShopInfo();
}

FAbilityShopInfo UTShopDataAsset::FindAbilityShopInfoByTag(const FGameplayTag& AbilityTag)
{
	for (const FAbilityShopInfo& AbilityInfo : AbilityShopInfo)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}

	return FAbilityShopInfo();
}

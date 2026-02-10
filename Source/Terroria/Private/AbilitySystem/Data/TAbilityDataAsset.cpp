// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Data/TAbilityDataAsset.h"

FTAbilityData UTAbilityDataAsset::FindAbilityDataByTag(const FGameplayTag& AttributeTag) const
{
	for (const FTAbilityData& AbilityData : AbilityDataContainer)
	{
		if (AbilityData.AbilityTag.MatchesTagExact(AttributeTag))
		{
			return AbilityData;
		}
	}

	return FTAbilityData();
}

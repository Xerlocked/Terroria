// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Data/TAttributeDataAsset.h"

FTAttributeData UTAttributeDataAsset::FindAttributeDataByTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FTAttributeData& AttributeData : AttributeDataContainer)
	{
		if (AttributeData.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeData;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute data not found: %s"), *AttributeTag.ToString());
	}
	
	return FTAttributeData();
}

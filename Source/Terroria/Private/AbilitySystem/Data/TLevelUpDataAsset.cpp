// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Data/TLevelUpDataAsset.h"

int32 UTLevelUpDataAsset::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	
	while (bSearching)
	{
		if (LevelUpsInformation.Num() - 1 <= Level) return Level;
		if (XP >= LevelUpsInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	
	return Level;
}

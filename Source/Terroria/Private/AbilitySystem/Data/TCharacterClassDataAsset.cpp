// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Data/TCharacterClassDataAsset.h"

FCharacterClassInfo UTCharacterClassDataAsset::GetCharacterClassInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}

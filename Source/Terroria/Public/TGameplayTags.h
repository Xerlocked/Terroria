// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Native gameplay tags
 */

struct FTGameplayTags
{
public:
	static const FTGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Agility;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Luck;

	FGameplayTag Attribute_Secondary_AttackPower;
	FGameplayTag Attribute_Secondary_MagicPower;
	FGameplayTag Attribute_Secondary_AttackSpeed;
	FGameplayTag Attribute_Secondary_CastSpeed;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_HealthDrain;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_MovementSpeed;


	FGameplayTag Input_Mouse_LMB;
	FGameplayTag Input_Mouse_RMB;
	FGameplayTag Input_Keyboard_Q;
	FGameplayTag Input_Keyboard_W;
	FGameplayTag Input_Keyboard_E;
	FGameplayTag Input_Keyboard_R;
	FGameplayTag Input_Keyboard_Space;
	
private:
	static FTGameplayTags GameplayTags;
};

// Copyright 2025 @xerlock. All Rights Reserved.


#include "TGameplayTags.h"

#include "GameplayTagsManager.h"

FTGameplayTags FTGameplayTags::GameplayTags;

void FTGameplayTags::InitializeNativeGameplayTags()
{
	// Primary tags
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Strength"),
		FString("Attack Power, Max Health"));

	GameplayTags.Attribute_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Agility"),
		FString("Attack Speed, Cast Speed"));

	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"),
		FString("Magic Power, Max Mana"));

	GameplayTags.Attribute_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Luck"),
		FString("CriticalHitChance, CriticalHitDamage"));

	// Secondary tags
	GameplayTags.Attribute_Secondary_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.AttackPower"),
			FString("40 + 2.25 * (Level - 1) + Str * 5"));
	
	GameplayTags.Attribute_Secondary_MagicPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.MagicPower"),
			FString("10 + 5.5 * (Level - 1) + Int * 12"));
	
	GameplayTags.Attribute_Secondary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.AttackSpeed"),
			FString("0.6 * (1 + 0.035 * (Level - 1)) * (1 + Agi * 0.005)"));
	
	GameplayTags.Attribute_Secondary_CastSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.CastSpeed"),
			FString("0.55 * ( 1 + 0.045 * (Level - 1)) * ( 1 + Agi * 0.005)"));
	
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.CriticalHitChance"),
			FString("(Luk * 0.005)"));
	
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.CriticalHitDamage"),
			FString("100 + (Luk * 1.5)"));
	
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.MaxHealth"),
			FString("120 + 85 * (Level - 1)"));
	
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.MaxMana"),
			FString("5 + 25 * (Level - 1)"));
	
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.HealthRegeneration"),
			FString("7 + 0.55 * (Level -1)"));
	
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.ManaRegeneration"),
			FString("1 + 1.35 * (Level - 1)"));
	
	GameplayTags.Attribute_Secondary_HealthDrain = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.HealthDrain"),
			FString("Item add"));
	
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.Armor"),
			FString("33 + 2.5 * (Level - 1)"));
	
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.BlockChance"),
			FString("Item add"));
	
	GameplayTags.Attribute_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Secondary.MovementSpeed"),
			FString("600"));
}

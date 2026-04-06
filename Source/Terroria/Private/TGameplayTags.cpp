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

	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.Armor"),
		FString("33 + 2.5 * (Level - 1)"));

	GameplayTags.Attribute_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MovementSpeed"),
		FString("600"));

	GameplayTags.Attribute_Secondary_CDR = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CDR"),
		FString("Cooldown Reduction"));

	// Meta
	GameplayTags.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Meta.IncomingXP"),
		FString("IncomingXP"));

	// Abilities
	GameplayTags.Ability_Common_PrimaryAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Common.PrimaryAttack"),
		FString("PrimaryAttack"));

	GameplayTags.Ability_Common_Flash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Common.Flash"),
		FString("Flash"));

	GameplayTags.Ability_Dark_LightingBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Dark.LightingBolt"),
		FString("Dark LightingBolt"));

	GameplayTags.Ability_Dark_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Dark.Shield"),
		FString("Dark Shield"));

	GameplayTags.Ability_Dark_Shockwave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Dark.Shockwave"),
		FString("Shockwave"));

	GameplayTags.Ability_Dark_BlackHole = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Dark.BlackHole"),
		FString("BlackHole"));

	GameplayTags.Ability_Boss_Laser = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.Laser"),
		FString("Boss Laser"));

	GameplayTags.Ability_Boss_JumpSlam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.JumpSlam"),
		FString("Boss JumpSlam"));

	GameplayTags.Ability_Boss_SummonMelee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.SummonMelee"),
		FString("Boss SummonMelee"));

	GameplayTags.Ability_Boss_Shockwave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.Shockwave"),
		FString("Boss Shockwave"));

	GameplayTags.Ability_Boss_EnterEnrage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.EnterEnrage"),
		FString("Boss EnterEnrage"));

	GameplayTags.Ability_Boss_Enraged_LaserSpin = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.Enraged.LaserSpin"),
		FString("Boss Enraged LaserSpin"));

	GameplayTags.Ability_Boss_Enraged_JumpSlamTriple = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.Enraged.JumpSlamTriple"),
		FString("Boss Enraged JumpSlamTriple"));

	GameplayTags.Ability_Boss_Enraged_SummonMixed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Boss.Enraged.SummonMixed"),
		FString("Boss Enraged SummonMixed"));


	// Cooldown
	GameplayTags.Cooldown_Common_Flash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Common.Flash"),
		FString("Flash Cooldown"));

	GameplayTags.Cooldown_Dark_LightingBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Dark.LightingBolt"),
		FString("Dark LightingBolt Cooldown"));

	GameplayTags.Cooldown_Dark_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Dark.Shield"),
		FString("Dark Shield Cooldown"));

	GameplayTags.Cooldown_Dark_Shockwave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Dark.Shockwave"),
		FString("Shockwave Cooldown"));

	GameplayTags.Cooldown_Dark_BlackHole = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Dark.BlackHole"),
		FString("BlackHole Cooldown"));

	GameplayTags.Cooldown_Attack_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Attack.Melee"),
		FString("Melee Attack Cooldown"));

	GameplayTags.Cooldown_Attack_Ranged = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Attack.Ranged"),
		FString("Ranged Attack Cooldown"));


	GameplayTags.Cooldown_Boss_Laser = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Boss.Laser"),
		FString("Boss Laser Cooldown"));

	GameplayTags.Cooldown_Boss_JumpSlam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Boss.JumpSlam"),
		FString("Boss JumpSlam Cooldown"));

	GameplayTags.Cooldown_Boss_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Boss.Summon"),
		FString("Boss Summon Cooldown"));

	GameplayTags.Cooldown_Boss_Enraged_LaserSpin = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Boss.Enraged.LaserSpin"),
		FString("Boss Enraged LaserSpin Cooldown"));

	GameplayTags.Cooldown_Boss_Enraged_JumpSlamTriple = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Boss.Enraged.JumpSlamTriple"),
		FString("Boss Enraged JumpSlamTriple Cooldown"));

	GameplayTags.Cooldown_Boss_Enraged_SummonMixed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Boss.Enraged.SummonMixed"),
		FString("Boss Enraged SummonMixed Cooldown"));

	GameplayTags.Block_Ability_Boss_Laser = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Ability.Boss.Laser"),
		FString("Block Boss Laser Ability"));

	GameplayTags.Block_Ability_Boss_JumpSlam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Ability.Boss.JumpSlam"),
		FString("Block Boss JumpSlam Ability"));

	GameplayTags.Block_Ability_Boss_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Ability.Boss.Summon"),
		FString("Block Boss Summon Ability"));

	GameplayTags.Block_Ability_Boss_NormalPattern = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Ability.Boss.NormalPattern"),
		FString("Block Boss Normal Pattern"));

	GameplayTags.Block_Ability_Boss_EnragedPattern = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Ability.Boss.EnragedPattern"),
		FString("Block Boss Enraged Pattern"));

	// Input Action Tags
	GameplayTags.Input_Mouse_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Mouse.LMB"),
		FString("Left Mouse Button"));

	GameplayTags.Input_Mouse_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Mouse.RMB"),
		FString("Right Mouse Button"));

	GameplayTags.Input_Mouse_Wheel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Mouse.Wheel"),
		FString("Mouse Wheel"));

	GameplayTags.Input_Keyboard_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.Q"),
		FString("Keyboard Q"));

	GameplayTags.Input_Keyboard_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.W"),
		FString("Keyboard W"));

	GameplayTags.Input_Keyboard_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.E"),
		FString("Keyboard E"));

	GameplayTags.Input_Keyboard_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.R"),
		FString("Keyboard R"));

	GameplayTags.Input_Keyboard_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.Space"),
		FString("Keyboard Space bar"));

	GameplayTags.Input_Keyboard_P = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.P"),
		FString("Keyboard P"));

	GameplayTags.Input_Keyboard_F = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.F"),
		FString("Keyboard F"));

	GameplayTags.Input_Keyboard_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Keyboard.1"),
		FString("Keyboard 1"));

	// Damage
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage"));
	GameplayTags.Damage_Type_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Type.Physical"),
		FString("Physical Damage")
	);
	GameplayTags.Damage_Type_Spell = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Type.Spell"),
		FString("Spell Damage")
	);

	// Effect
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Effects HitReact"));

	GameplayTags.Effects_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Death"),
		FString("Effects Death"));


	// State Tags
	GameplayTags.State_Cast_Rooted = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Cast.Rooted"),
		FString("Rooted for casting"));

	GameplayTags.State_Attacking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Attacking"),
		FString("Attack State"));

	GameplayTags.State_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Death"),
		FString("Death State"));

	GameplayTags.State_Stunned = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Stunned"),
		FString("Stunned State"));

	GameplayTags.State_Idle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Idle"),
		FString("Idle State"));

	GameplayTags.State_Search = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Search"),
		FString("Search State"));

	GameplayTags.State_Wander = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Wander"),
		FString("Wander State"));

	GameplayTags.State_Alert = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Alert"),
		FString("Alert State"));

	GameplayTags.State_Chase = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Chase"),
		FString("Chase State"));

	GameplayTags.State_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Attack"),
		FString("Attack State"));

	GameplayTags.State_Return = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Return"),
		FString("Return State"));

	GameplayTags.State_Boss_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Normal"),
		FString("Boss Normal State"));

	GameplayTags.State_Boss_Enraged = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Enraged"),
		FString("Boss Enraged State"));

	GameplayTags.State_Boss_Combat = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Combat"),
		FString("Boss Combat State"));

	GameplayTags.State_Boss_Idle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Idle"),
		FString("Boss Idle State"));

	GameplayTags.State_Boss_Casting = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Casting"),
		FString("Boss Casting State"));

	GameplayTags.State_Boss_Jumping = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Jumping"),
		FString("Boss Jumping State"));

	GameplayTags.State_Boss_Summoning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Summoning"),
		FString("Boss Summoning State"));

	GameplayTags.State_Boss_Invulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Invulnerable"),
		FString("Boss Invulnerable State"));

	GameplayTags.State_Boss_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Boss.Dead"),
		FString("Boss Dead State"));

	// AI Tags
	GameplayTags.AI_HasTarget = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.HasTarget"),
		FString("AI Has Target"));

	GameplayTags.AI_LostTarget = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.LostTarget"),
		FString("AI Lost Target"));

	GameplayTags.AI_InDetectRange = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.InDetectRange"),
		FString("AI In Detect Range"));

	GameplayTags.AI_InAttackRange = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.InAttackRange"),
		FString("AI In Attack Range"));

	GameplayTags.AI_ShouldKeepDistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.ShouldKeepDistance"),
		FString("AI Should Keep Distance"));

	GameplayTags.AI_Type_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.Type.Melee"),
		FString("AI Type Melee"));

	GameplayTags.AI_Type_Ranged = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AI.Type.Ranged"),
		FString("AI Type Ranged"));

	// Event
	GameplayTags.Event_Kill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Kill"),
		FString("Event Kill"));

	GameplayTags.Event_Boss_Health_Below50 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Health.Below50"),
		FString("Boss Health Below 50%"));

	GameplayTags.Event_Boss_Pattern_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Pattern.Start"),
		FString("Boss Pattern Start"));

	GameplayTags.Event_Boss_Pattern_End = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Pattern.End"),
		FString("Boss Pattern End"));

	GameplayTags.Event_Boss_Enrage_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Enrage.Start"),
		FString("Boss Enrage Start"));

	GameplayTags.Event_Boss_Enrage_End = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Enrage.End"),
		FString("Boss Enrage End"));

	GameplayTags.Event_Boss_Laser = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Laser"),
		FString("Boss Laser Event"));

	GameplayTags.Event_Boss_JumpSlam = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.JumpSlam"),
		FString("Boss JumpSlam Event"));

	GameplayTags.Event_Boss_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Summon"),
		FString("Boss Summon Event"));

	GameplayTags.Event_Boss_Shockwave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Shockwave"),
		FString("Boss Shockwave Event"));

	GameplayTags.Event_Boss_Enraged_LaserSpin = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Enraged.LaserSpin"),
		FString("Boss Enraged LaserSpin Event"));

	GameplayTags.Event_Boss_Enraged_JumpSlamTriple = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Enraged.JumpSlamTriple"),
		FString("Boss Enraged JumpSlamTriple Event"));

	GameplayTags.Event_Boss_Enraged_SummonMixed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Boss.Enraged.SummonMixed"),
		FString("Boss Enraged SummonMixed Event"));
}

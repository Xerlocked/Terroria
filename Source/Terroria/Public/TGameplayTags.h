// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Native gameplay tags
 */

struct FTGameplayTags
{
	static const FTGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Agility;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Luck;

	FGameplayTag Attribute_Secondary_AttackPower;
	FGameplayTag Attribute_Secondary_MagicPower;
	FGameplayTag Attribute_Secondary_AttackSpeed;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_MovementSpeed;
	FGameplayTag Attribute_Secondary_CDR;

	FGameplayTag Attribute_Meta_IncomingXP;

	FGameplayTag Ability_Common_PrimaryAttack;
	FGameplayTag Ability_Common_Flash;

	FGameplayTag Ability_Dark_LightingBolt;
	FGameplayTag Ability_Dark_Shield;
	FGameplayTag Ability_Dark_Shockwave;
	FGameplayTag Ability_Dark_BlackHole;

	FGameplayTag Ability_Boss_Laser;
	FGameplayTag Ability_Boss_JumpSlam;
	FGameplayTag Ability_Boss_SummonMelee;
	FGameplayTag Ability_Boss_Shockwave;
	FGameplayTag Ability_Boss_EnterEnrage;
	FGameplayTag Ability_Boss_Enraged_LaserSpin;
	FGameplayTag Ability_Boss_Enraged_JumpSlamTriple;
	FGameplayTag Ability_Boss_Enraged_SummonMixed;

	FGameplayTag Cooldown_Common_Flash;
	FGameplayTag Cooldown_Dark_LightingBolt;
	FGameplayTag Cooldown_Dark_Shield;
	FGameplayTag Cooldown_Dark_Shockwave;
	FGameplayTag Cooldown_Dark_BlackHole;
	FGameplayTag Cooldown_Attack_Melee;
	FGameplayTag Cooldown_Attack_Ranged;

	FGameplayTag Cooldown_Boss_Laser;
	FGameplayTag Cooldown_Boss_JumpSlam;
	FGameplayTag Cooldown_Boss_Summon;
	FGameplayTag Cooldown_Boss_Enraged_LaserSpin;
	FGameplayTag Cooldown_Boss_Enraged_JumpSlamTriple;
	FGameplayTag Cooldown_Boss_Enraged_SummonMixed;

	FGameplayTag Block_Ability_Boss_Laser;
	FGameplayTag Block_Ability_Boss_JumpSlam;
	FGameplayTag Block_Ability_Boss_Summon;
	FGameplayTag Block_Ability_Boss_NormalPattern;
	FGameplayTag Block_Ability_Boss_EnragedPattern;

	FGameplayTag Input_Mouse_LMB;
	FGameplayTag Input_Mouse_RMB;
	FGameplayTag Input_Mouse_Wheel;
	FGameplayTag Input_Keyboard_Q;
	FGameplayTag Input_Keyboard_W;
	FGameplayTag Input_Keyboard_E;
	FGameplayTag Input_Keyboard_R;
	FGameplayTag Input_Keyboard_Space;
	FGameplayTag Input_Keyboard_P;
	FGameplayTag Input_Keyboard_F;
	FGameplayTag Input_Keyboard_1;

	FGameplayTag Damage;
	FGameplayTag Damage_Type_Physical;
	FGameplayTag Damage_Type_Spell;

	FGameplayTag Effects_HitReact;
	FGameplayTag Effects_Death;

	FGameplayTag State_Cast_Rooted;
	FGameplayTag State_Attacking;
	FGameplayTag State_Death;
	FGameplayTag State_Stunned;
	FGameplayTag State_Idle;
	FGameplayTag State_Search;
	FGameplayTag State_Wander;
	FGameplayTag State_Alert;
	FGameplayTag State_Chase;
	FGameplayTag State_Attack;
	FGameplayTag State_Return;

	FGameplayTag State_Boss_Normal;
	FGameplayTag State_Boss_Enraged;
	FGameplayTag State_Boss_Combat;
	FGameplayTag State_Boss_Idle;
	FGameplayTag State_Boss_Casting;
	FGameplayTag State_Boss_Jumping;
	FGameplayTag State_Boss_Summoning;
	FGameplayTag State_Boss_Invulnerable;
	FGameplayTag State_Boss_Dead;

	FGameplayTag AI_HasTarget;
	FGameplayTag AI_LostTarget;
	FGameplayTag AI_InDetectRange;
	FGameplayTag AI_InAttackRange;
	FGameplayTag AI_ShouldKeepDistance;

	FGameplayTag AI_Type_Melee;
	FGameplayTag AI_Type_Ranged;

	FGameplayTag Event_Boss_Health_Below50;
	FGameplayTag Event_Boss_Pattern_Start;
	FGameplayTag Event_Boss_Pattern_End;
	FGameplayTag Event_Boss_Enrage_Start;
	FGameplayTag Event_Boss_Enrage_End;
	FGameplayTag Event_Boss_Laser;
	FGameplayTag Event_Boss_JumpSlam;
	FGameplayTag Event_Boss_Shockwave;
	FGameplayTag Event_Boss_Summon;
	FGameplayTag Event_Boss_Enraged_LaserSpin;
	FGameplayTag Event_Boss_Enraged_JumpSlamTriple;
	FGameplayTag Event_Boss_Enraged_SummonMixed;
	FGameplayTag Event_Kill;

private:
	static FTGameplayTags GameplayTags;
};

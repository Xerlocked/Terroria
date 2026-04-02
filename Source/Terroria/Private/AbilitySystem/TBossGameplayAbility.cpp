// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TBossGameplayAbility.h"

#include "TGameplayTags.h"

UTBossGameplayAbility::UTBossGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	// Blocked while dead or already casting
	ActivationBlockedTags.AddTag(FTGameplayTags::Get().State_Boss_Dead);
	ActivationBlockedTags.AddTag(FTGameplayTags::Get().State_Boss_Casting);
}

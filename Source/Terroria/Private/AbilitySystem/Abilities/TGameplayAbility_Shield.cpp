// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Abilities/TGameplayAbility_Shield.h"

#include "AbilitySystemComponent.h"

UTGameplayAbility_Shield::UTGameplayAbility_Shield()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTGameplayAbility_Shield::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (HasAuthority(&CurrentActivationInfo) && PassiveEffectClass)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		if (ASC)
		{
			if (PassiveEffectHandle.IsValid())
			{
				ASC->RemoveActiveGameplayEffect(PassiveEffectHandle);
			}

			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
				PassiveEffectClass, GetAbilityLevel(), ContextHandle);

			if (SpecHandle.IsValid())
			{
				PassiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void UTGameplayAbility_Shield::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (HasAuthority(&CurrentActivationInfo) && ActiveEffectClass)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		if (ASC)
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
				ActiveEffectClass, GetAbilityLevel(), ContextHandle);

			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

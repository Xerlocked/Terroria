// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssetTagsEvent, const FGameplayTagContainer&)

/**
 * 
 */
UCLASS()
class TERRORIA_API UTAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void BindAbilityActorInfo();

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	void HeldAbilityInputTag(const FGameplayTag& InputTag);

	void ReleaseAbilityInputTag(const FGameplayTag& InputTag);
	
	FOnAssetTagsEvent AssetTagsEvent;
	
protected:

	UFUNCTION(Client, Reliable)
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const;
};

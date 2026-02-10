// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssetTagsEvent, const FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityGivenEvent, UTAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)

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
	bool bStartupAbilitiesGiven = false;
	
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	void HeldAbilityInputTag(const FGameplayTag& InputTag);

	void ReleaseAbilityInputTag(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& Spec);


	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	FOnAssetTagsEvent AssetTagsEvent;

	FAbilityGivenEvent AbilityGivenEvent;
	
protected:

	UFUNCTION(Client, Reliable)
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const;

	virtual void OnRep_ActivateAbilities() override;
};

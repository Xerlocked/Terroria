// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TGameplayAbility.h"
#include "TGameplayAbility_Shield.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UTGameplayAbility_Shield : public UTGameplayAbility
{
	GENERATED_BODY()

public:
	UTGameplayAbility_Shield();

	/* Passive Effect */
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ApplyActiveEffect(UAbilitySystemComponent* AbilitySystemComponent);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> PassiveEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> ActiveEffectClass;

private:
	FActiveGameplayEffectHandle PassiveEffectHandle;
};

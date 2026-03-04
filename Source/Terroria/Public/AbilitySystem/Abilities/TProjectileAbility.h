// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TGameplayAbility.h"
#include "TProjectileAbility.generated.h"

class ATBaseProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class TERRORIA_API UTProjectileAbility : public UTGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& TargetLocation, const FName SocketName);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATBaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat ProjectileDamage = FScalableFloat();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

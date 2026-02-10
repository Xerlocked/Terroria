// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TGameplayAbility.h"
#include "TGameplayAbility_BlackHole.generated.h"

class ATSphereAreaEffector;
class AGameplayAbilityTargetActor;
/**
 * 
 */
UCLASS()
class TERRORIA_API UTGameplayAbility_BlackHole : public UTGameplayAbility
{
	GENERATED_BODY()

public:
	UTGameplayAbility_BlackHole();

	UFUNCTION(BlueprintCallable, Category = "Ability|BlackHole")
	void ExecuteBlackHole();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATSphereAreaEffector> SphereEffectorClass;


	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data);
};

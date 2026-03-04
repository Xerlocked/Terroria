// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TGameplayAbility.h"
#include "TGameplayAbility_ShockWave.generated.h"

class UGameplayEffect;

UCLASS()
class TERRORIA_API UTGameplayAbility_ShockWave : public UTGameplayAbility
{
	GENERATED_BODY()

public:
	UTGameplayAbility_ShockWave();

protected:
	/** Effect to apply to targets hit by the wave */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Object Types to trace for (e.g., Pawn, PhysicsBody, Monster) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;
};

// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TGameplayAbility.h"
#include "TGameplayAbility_ShockWave.generated.h"

class UGameplayEffect;

/**
 * Ability that releases a seismic wave in a straight line towards a target location.
 * Deals damage to all enemies caught in the path.
 */
UCLASS()
class TERRORIA_API UTGameplayAbility_ShockWave : public UTGameplayAbility
{
	GENERATED_BODY()

public:
	UTGameplayAbility_ShockWave();

protected:
	/**
	 * Executes the Seismic Wave logic: performs a box trace towards the TargetLocation
	 * and applies the DamageEffect to hit targets.
	 * 
	 * @param TargetLocation The location to aim the wave at (e.g., mouse cursor position).
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|ShockWave")
	void ExecuteShockWave(const FVector& TargetLocation);

	/** Effect to apply to targets hit by the wave */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	TSubclassOf<AActor> ShockWaveProjectileClass;

	/** Total length of the seismic wave */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	float WaveRange = 1000.0f;

	/** Width of the seismic wave (total width) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	float WaveWidth = 150.0f;

	/** Object Types to trace for (e.g., Pawn, PhysicsBody) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;

	/** Debug draw duration (0 to disable) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	float DebugDrawDuration = 2.0f;
};

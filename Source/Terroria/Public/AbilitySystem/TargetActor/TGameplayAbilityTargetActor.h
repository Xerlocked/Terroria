// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TGameplayAbilityTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API ATGameplayAbilityTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ATGameplayAbilityTargetActor();

	virtual void Tick(float DeltaSeconds) override;

	bool TraceUnderCursor(FHitResult& TraceHitResult) const;

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors();

protected:
	UPROPERTY()
	TObjectPtr<AGameplayAbilityWorldReticle> MyReticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "GASGameplayAbility")
	float TraceRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
	FHitResult TraceHitResults;
};

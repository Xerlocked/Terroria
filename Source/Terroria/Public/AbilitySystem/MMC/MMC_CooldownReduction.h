// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CooldownReduction.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UMMC_CooldownReduction : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_CooldownReduction();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition ReductionDef;
};

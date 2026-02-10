// Copyright 2025 @xerlock. All Rights Reserved.

#include "AbilitySystem/MMC/MMC_CooldownReduction.h"

#include "AbilitySystem/TAttributeSet.h"
#include "AbilitySystem/TGameplayAbility.h"

UMMC_CooldownReduction::UMMC_CooldownReduction()
{
	ReductionDef.AttributeToCapture =
		UTAttributeSet::GetCooldownReductionAttribute();

	ReductionDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	ReductionDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ReductionDef);
}

float UMMC_CooldownReduction::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UTGameplayAbility* Ability = Cast<UTGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}

	const float BaseCooldown = Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel());

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float CooldownReduction = 0.0f;
	GetCapturedAttributeMagnitude(ReductionDef, Spec, EvaluateParameters, CooldownReduction);

	const float FinalCooldown = BaseCooldown * (1.0f - (CooldownReduction / 100.0f));

	return FMath::Max(0.0f, FinalCooldown);
}

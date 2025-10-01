// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "Interface/StatusInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	const IStatusInterface* Status = Cast<IStatusInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = Status->GetPlayerLevel();

	return 120.0f + 85.0f * (PlayerLevel - 1);
}

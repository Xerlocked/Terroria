// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "Interface/StatusInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	const IStatusInterface* Status = Cast<IStatusInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = Status->GetPlayerLevel();

	return 5.0f + 25.0f * (PlayerLevel - 1);
}

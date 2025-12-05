// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "Interface/CharacterData.h"

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

	int32 PlayerLevel = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCharacterData>())
	{
		PlayerLevel = ICharacterData::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 120.0f + 85.0f * (PlayerLevel - 1);
}

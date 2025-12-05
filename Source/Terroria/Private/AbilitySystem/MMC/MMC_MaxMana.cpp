// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "Interface/CharacterData.h"

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

	int32 PlayerLevel = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCharacterData>())
	{
		PlayerLevel = ICharacterData::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}
	
	return 5.0f + 25.0f * (PlayerLevel - 1);
}

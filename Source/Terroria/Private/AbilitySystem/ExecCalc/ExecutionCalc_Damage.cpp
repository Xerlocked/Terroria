// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecutionCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAttributeSet.h"

struct TDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	TDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTAttributeSet, Armor, Target, false);
	}
};

static const TDamageStatics& DamageStatics()
{
	static TDamageStatics DStatics;
	return DStatics;
}

UExecutionCalc_Damage::UExecutionCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecutionCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceAbility = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbility = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceAbility ? SourceAbility->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAbility ? TargetAbility->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float FinalDamage = Spec.GetSetByCallerMagnitude(FTGameplayTags::Get().Damage);

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParams, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	FinalDamage -= TargetArmor;
	FinalDamage = FMath::Max<float>(FinalDamage, -0.5f);

	const FGameplayModifierEvaluatedData EvaluatedData(UTAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, FinalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

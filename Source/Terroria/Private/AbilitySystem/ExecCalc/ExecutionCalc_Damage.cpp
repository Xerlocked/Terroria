// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecutionCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAttributeSet.h"

struct TDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	TDamageStatics()
	{
		// Victim
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTAttributeSet, Armor, Target, false);

		// Instigator
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTAttributeSet, MagicPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTAttributeSet, CriticalHitDamage, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecutionCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FTGameplayTags& GameplayTags = FTGameplayTags::Get();

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

	float SourceAttackPower = 0.f;
	float SourceMagicPower = 0.f;
	float SourceCriticalHitChance = 0.f;
	float SourceCriticalHitDamage = 0.f;
	float TargetArmor = 0.f;

	// 스탯 값 추출
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParams, TargetArmor);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluateParams,
	                                                           SourceAttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicPowerDef, EvaluateParams,
	                                                           SourceMagicPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParams,
	                                                           SourceCriticalHitChance);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParams,
	                                                           SourceCriticalHitDamage);

	// 하한선 보정
	SourceAttackPower = FMath::Max<float>(SourceAttackPower, 0.f);
	SourceMagicPower = FMath::Max<float>(SourceMagicPower, 0.f);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// 기본 스킬 데미지 (평타일 경우 0)
	const float BaseDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Damage);
	float FinalDamage;

	if (EvaluateParams.SourceTags->HasTagExact(GameplayTags.Damage_Type_Spell))
	{
		FinalDamage = BaseDamage + SourceMagicPower;
	}
	else
	{
		FinalDamage = BaseDamage + SourceAttackPower;
	}

	if (bool bIsCriticalHit = FMath::RandRange(0.f, 100.f) <= SourceCriticalHitChance)
	{
		const float CriticalMultiplier = 1.f + (SourceCriticalHitDamage / 100.f);
		FinalDamage = FinalDamage * CriticalMultiplier;
	}
	else
	{
		// 방어구 공식: 방어구 수치 / (방어구 수치 + 상수)
		constexpr float ArmorConstant = 100.f;
		const float EffectiveArmor = FMath::Max<float>(TargetArmor, 0.f);
		const float DamageMultiplier = ArmorConstant / (EffectiveArmor + ArmorConstant);

		FinalDamage = FinalDamage * DamageMultiplier;
	}

	FinalDamage = FMath::Max<float>(FinalDamage, 0.f);

	const FGameplayModifierEvaluatedData EvaluatedData(UTAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, FinalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

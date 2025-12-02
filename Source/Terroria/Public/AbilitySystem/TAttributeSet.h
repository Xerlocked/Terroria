// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "TAttributeSet.generated.h"

USTRUCT()
struct FEffectProperty
{
	GENERATED_BODY()
	FEffectProperty() {}

	FGameplayEffectContextHandle ContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* InstigatorASC = nullptr;

	UPROPERTY()
	AActor* InstigatorAvatarActor = nullptr;

	UPROPERTY()
	AController* InstigatorController = nullptr;

	UPROPERTY()
	ACharacter* InstigatorCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

template<class T>
using TStaticFuncPtr = TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class TERRORIA_API UTAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
public:

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	//~  Begin Primary Attributes (핵심 능력치)
	// 플레이어가 직접 포인트를 투자하는 능력치

	/* 힘: 공격력, 체력 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Character|Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Strength)

	/* 민첩: 공격 속도 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Character|Primary")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Agility)

	/* 지능: 마력, 마나 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Character|Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Intelligence)

	/* 운: 치명타율, 치명타 데미지 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Luck, Category = "Character|Primary")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Luck)
	//~ End Primary Attributes

	//~ Begin Secondary Attributes (2차 능력치)
	// 플레이어가 직접 포인트를 투자할 순 없지만, 주 능력치에 따라 상승

	/* 공격력 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Character|Secondary")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, AttackPower)

	/* 마력 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicPower, Category = "Character|Secondary")
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, MagicPower)

	/* 공격 속도 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Character|Secondary")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, AttackSpeed)

	/* 치명타율 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Character|Secondary")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, CriticalHitChance)

	/* 치명타 데미지 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Character|Secondary")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, CriticalHitDamage)
	//~ End Secondary Attributes

	//~ Begin Vital Attributes (생명 자원)

	/* 체력 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Character|Vital")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Health)

	/* 최대 체력 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Character|Vital")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, MaxHealth)

	/* 체력 재생 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Character|Vital")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, HealthRegeneration)

	/* 마나 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Character|Vital")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Mana)

	/* 최대 마나 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Character|Vital")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, MaxMana)

	/* 마나 재생 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Character|Vital")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, ManaRegeneration)
	//~ End Vital Attributes

	//~ Begin Unique Attributes
	/* 이동 속도 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Character|Unique")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, MovementSpeed)
	
	/* 방어력 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Character|Unique")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, Armor)
	//~ End Unique Attributes

	//~ Begin Meta Attributes

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS_BASIC(UTAttributeSet, IncomingDamage);
	
	//~ End Meat Attributes

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& OldValue) const;


	//------------------

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MagicPower(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;

	//------------------
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const;

	//------------------

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const;

private:
	void GetGameplayEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperty& Property);
};

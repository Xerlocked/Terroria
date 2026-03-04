// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "TGameContainers.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterData.h"
#include "TCharacterBase.generated.h"

struct FGameplayTag;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract)
class TERRORIA_API ATCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICharacterData
{
	GENERATED_BODY()

public:
	ATCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual int32 GetPlayerLevel_Implementation() const override;

	UAttributeSet* GetAttributeSet() { return AttributeSet; }

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override;

	virtual ECharacterClass GetCharacterClass_Implementation() const override;

protected:
	virtual void SetupAbilityActorInfo();

	virtual void InitializeDefaultAttributes() const;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;

	void AddCharacterAbilities();

	virtual void SpawnDropItem();

	UFUNCTION(BlueprintCallable)
	virtual void EnableRagdoll();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Death")
	void HandleDeath();

	virtual void OnDeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attributes")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Item")
	TArray<FDropItem> DropTables;

	UPROPERTY(EditDefaultsOnly, Category = "Anim|Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

private:
	UPROPERTY(EditAnywhere, Category = "GAS|Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS|Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
};

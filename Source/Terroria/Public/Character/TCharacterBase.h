// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterData.h"
#include "Interface/StatusInterface.h"
#include "TCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class TERRORIA_API ATCharacterBase : public ACharacter, public IAbilitySystemInterface, public IStatusInterface, public ICharacterData
{
	GENERATED_BODY()

public:
	ATCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual int32 GetPlayerLevel() const override;
	
	UAttributeSet* GetAttributeSet() { return AttributeSet; }
	
protected:
	virtual void SetupAbilityActorInfo();

	virtual void InitializeDefaultAttributes() const;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;
	
	void AddCharacterAbilities();
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName;
	
private:
	UPROPERTY(EditAnywhere, Category = "GAS|Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};

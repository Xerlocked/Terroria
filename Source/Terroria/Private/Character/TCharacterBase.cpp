// Copyright 2025 @xerlock. All Rights Reserved.


#include "Character/TCharacterBase.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"

ATCharacterBase::ATCharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ATCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

int32 ATCharacterBase::GetPlayerLevel() const
{
	return 0;
}

void ATCharacterBase::SetupAbilityActorInfo()
{
	
}

void ATCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level,  ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(PrimaryAttributes, 1.0f);
	ApplyEffectToSelf(SecondaryAttributes, 1.0f);
	ApplyEffectToSelf(VitalAttributes, 1.0f);
}

void ATCharacterBase::AddCharacterAbilities()
{
	UTAbilitySystemComponent* TASC = CastChecked<UTAbilitySystemComponent>(AbilitySystemComponent.Get());
	if (!HasAuthority()) return;

	TASC->AddCharacterAbilities(StartupAbilities);
}


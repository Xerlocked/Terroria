// Copyright 2025 @xerlock. All Rights Reserved.


#include "Player/TPlayerState.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"

ATPlayerState::ATPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTAttributeSet>(TEXT("AttributeSet"));
	
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ATPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

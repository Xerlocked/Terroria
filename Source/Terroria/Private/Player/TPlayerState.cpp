// Copyright 2025 @xerlock. All Rights Reserved.


#include "Player/TPlayerState.h"

#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"
#include "Net/UnrealNetwork.h"

ATPlayerState::ATPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTAttributeSet>(TEXT("AttributeSet"));

	SetNetUpdateFrequency(100.f);
}

void ATPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPlayerState, Level);
	DOREPLIFETIME(ATPlayerState, XP);
	DOREPLIFETIME(ATPlayerState, AttributePoint);
	DOREPLIFETIME(ATPlayerState, Gold);
}

UAbilitySystemComponent* ATPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ATPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATPlayerState::AddToAttributePoint(int32 InAttributePoint)
{
	AttributePoint += InAttributePoint;
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void ATPlayerState::AddToGold(int32 InGold)
{
	Gold += InGold;
	OnGoldChangedDelegate.Broadcast(Gold);
}

void ATPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ATPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void ATPlayerState::OnRep_Gold(int32 OldGold)
{
	OnGoldChangedDelegate.Broadcast(Gold);
}

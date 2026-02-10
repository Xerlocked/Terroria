// Copyright 2025 @xerlock. All Rights Reserved.


#include "Character/TCharacterBase.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"
#include "Components/CapsuleComponent.h"

ATCharacterBase::ATCharacterBase()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ATCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

int32 ATCharacterBase::GetPlayerLevel_Implementation() const
{
	return 0;
}

UAnimMontage* ATCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ATCharacterBase::Die()
{
	MulticastDeath();
}

ECharacterClass ATCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

void ATCharacterBase::MulticastDeath_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
}

void ATCharacterBase::SetupAbilityActorInfo()
{
}

void ATCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		EffectClass, Level, ContextHandle);

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
	if (!HasAuthority())
	{
		return;
	}

	TASC->AddCharacterAbilities(StartupAbilities);
	TASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

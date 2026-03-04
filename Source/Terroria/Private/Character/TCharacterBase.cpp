// Copyright 2025 @xerlock. All Rights Reserved.


#include "Character/TCharacterBase.h"

#include "TGameplayTags.h"
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
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			ASC->CancelAllAbilities();

			FGameplayTagContainer DeathAbilityContainer;
			DeathAbilityContainer.AddTag(FTGameplayTags::Get().Effects_Death);
			ASC->TryActivateAbilitiesByTag(DeathAbilityContainer);
		}
	}
}

ECharacterClass ATCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
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

void ATCharacterBase::SpawnDropItem()
{
}

void ATCharacterBase::EnableRagdoll()
{
	SetReplicateMovement(false);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}


	if (GetMesh())
	{
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->bBlendPhysics = true;
	}
}

void ATCharacterBase::HandleDeath_Implementation()
{
}

void ATCharacterBase::OnDeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		HandleDeath();
	}
}

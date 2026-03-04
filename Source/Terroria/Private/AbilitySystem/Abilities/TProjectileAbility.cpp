// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Abilities/TProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TGameplayTags.h"
#include "Actor/TBaseProjectile.h"
#include "Interface/CharacterData.h"

void UTProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTProjectileAbility::SpawnProjectile(const FVector& TargetLocation, const FName SocketName)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}

	if (GetAvatarActorFromActorInfo()->Implements<UCharacterData>())
	{
		const FVector FireSocketLocation = ICharacterData::Execute_GetWeaponSocketLocation(
			GetAvatarActorFromActorInfo(), SocketName);
		FRotator ProjectileRot = (TargetLocation - FireSocketLocation).Rotation();
		ProjectileRot.Pitch = 0.f;

		FTransform ProjectileTransform;
		ProjectileTransform.SetLocation(FireSocketLocation);
		ProjectileTransform.SetRotation(ProjectileRot.Quaternion());

		ATBaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATBaseProjectile>(ProjectileClass,
			ProjectileTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		const float DamageBaseValue = ProjectileDamage.GetValueAtLevel(GetAbilityLevel());

		const FTGameplayTags& GameplayTags = FTGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, DamageBaseValue);
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(ProjectileTransform);
	}
}

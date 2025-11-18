// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Abilities/TProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TGameplayTags.h"
#include "Actor/TBaseProjectile.h"
#include "Interface/CharacterData.h"

void UTProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTProjectileAbility::SpawnProjectile(const FVector& TargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	ICharacterData* CharacterData = Cast<ICharacterData>(GetAvatarActorFromActorInfo());
	if (CharacterData)
	{
		const FVector FireSocketLocation = CharacterData->GetWeaponSocketLocation();
		FRotator ProjectileRot = (TargetLocation - FireSocketLocation).Rotation();
		ProjectileRot.Pitch = 0.f;
		
		FTransform ProjectileTransform;
		ProjectileTransform.SetLocation(FireSocketLocation);
		ProjectileTransform.SetRotation(ProjectileRot.Quaternion());
		
		ATBaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATBaseProjectile>(ProjectileClass,
			ProjectileTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		const FTGameplayTags& GameplayTags = FTGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(ProjectileTransform);
	}
}

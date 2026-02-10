// Copyright 2025 @xerlock. All Rights Reserved.

#include "AbilitySystem/Abilities/TGameplayAbility_ShockWave.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TGameplayTags.h"
#include "Interface/CharacterData.h"


UTGameplayAbility_ShockWave::UTGameplayAbility_ShockWave()
{
	// Default hit types (can be changed in Blueprint defaults)
	HitObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	HitObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
}

void UTGameplayAbility_ShockWave::ExecuteShockWave(const FVector& TargetLocation)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	// 1. Calculate Direction and Box Transform
	const FVector StartLocation = AvatarActor->GetActorLocation();
	// Ignore Z difference for direction to keep it horizontal
	FVector Direction = (TargetLocation - StartLocation);
	Direction.Z = 0.f;
	Direction.Normalize();

	const FVector EndLocation = StartLocation + (Direction * WaveRange);
	const FRotator Orientation = Direction.Rotation();

	// Box half-extent (Length is covered by the trace start-end, so X extent can
	// be 0 or small, Y is Width/2, Z is height) For BoxTraceMulti, the Start->End
	// defines the sweep, and HalfSize defines the box at any point. We want a
	// flat box sweeping forward.
	FVector BoxHalfSize = FVector(0.f, WaveWidth * 0.5f, 50.0f); // 50 height arbitrary

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	TArray<FHitResult> OutHits;

	// 2. Perform Box Trace
	bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(), StartLocation, EndLocation, BoxHalfSize, Orientation,
		HitObjectTypes, false, /* bTraceComplex */
		ActorsToIgnore,
		DebugDrawDuration > 0.f
			? EDrawDebugTrace::ForDuration
			: EDrawDebugTrace::None,
		OutHits, true, /* bIgnoreSelf */
		FLinearColor::Red, FLinearColor::Green, DebugDrawDuration);

	// 3. Spawn Projectile only visual
	if (GetAvatarActorFromActorInfo()->Implements<UCharacterData>())
	{
		const FVector FireSocketLocation = ICharacterData::Execute_GetWeaponSocketLocation(
			GetAvatarActorFromActorInfo());
		FRotator ProjectileRot = (EndLocation - FireSocketLocation).Rotation();
		ProjectileRot.Pitch = 0.f;


		FTransform ProjectileTransform;
		ProjectileTransform.SetLocation(FireSocketLocation);
		ProjectileTransform.SetRotation(ProjectileRot.Quaternion());

		GetWorld()->SpawnActor<AActor>(ShockWaveProjectileClass, ProjectileTransform);
	}

	if (bHit)
	{
		UAbilitySystemComponent* SourceASC =
			GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC || !DamageEffectClass)
		{
			return;
		}

		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(AvatarActor);

		// Prepare Spec
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), EffectContext);

		// Optional: assign damage magnitude by Caller if needed, similar to
		// Projectile example
		const FTGameplayTags& GameplayTags = FTGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle, GameplayTags.Damage, 100.f); // Base damage example

		TSet<AActor*> ProcessedActors;

		// 3. Apply Damage
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !ProcessedActors.Contains(HitActor))
			{
				ProcessedActors.Add(HitActor);

				UAbilitySystemComponent* TargetASC =
					UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
				if (TargetASC)
				{
					SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),
					                                           TargetASC);
				}
			}
		}
	}
}

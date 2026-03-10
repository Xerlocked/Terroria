// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Abilities/TGameplayAbility_BlackHole.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Actor/TSphereAreaEffector.h"

UTGameplayAbility_BlackHole::UTGameplayAbility_BlackHole()
{
}

void UTGameplayAbility_BlackHole::ExecuteBlackHole()
{
	UAbilityTask_WaitTargetData* Task = UAbilityTask_WaitTargetData::WaitTargetData(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::UserConfirmed,
		TargetActorClass
	);

	AGameplayAbilityTargetActor* GenericActor = nullptr;
	if (Task->BeginSpawningActor(this, TargetActorClass, GenericActor))
	{
		Task->FinishSpawningActor(this, GenericActor);
	}
	else
	{
		Task->EndTask();
		return;
	}

	Task->ValidData.AddDynamic(this, &UTGameplayAbility_BlackHole::OnTargetDataReady);
	Task->Cancelled.AddDynamic(this, &UTGameplayAbility_BlackHole::OnTargetDataCancelled);

	Task->ReadyForActivation();
}

void UTGameplayAbility_BlackHole::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data)
{
	const FHitResult* HitResult = Data.Get(0)->GetHitResult();

	if (HitResult->bBlockingHit)
	{
		CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

		FVector TargetLocation = HitResult->Location;

		FHitResult GroundHit;

		FVector TraceStart = TargetLocation + FVector(0.f, 0.f, 100.f);
		FVector TraceEnd = TargetLocation - FVector(0.f, 0.f, 2000.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());
		if (HitResult->GetActor())
		{
			QueryParams.AddIgnoredActor(HitResult->GetActor());
		}

		bool bHitGround = GetWorld()->LineTraceSingleByChannel(
			GroundHit,
			TraceStart,
			TraceEnd,
			ECC_WorldStatic,
			QueryParams
		);

		FVector FinalLocation = bHitGround ? GroundHit.Location : TargetLocation;

		FTransform SpawnTransform = FTransform::Identity;
		SpawnTransform.SetLocation(FinalLocation);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetAvatarActorFromActorInfo());

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		const FTGameplayTags& GameplayTags = FTGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);

		ATSphereAreaEffector* Effector = GetWorld()->SpawnActorDeferred<ATSphereAreaEffector>(
			SphereEffectorClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		if (Effector)
		{
			Effector->EffectSpecHandle = SpecHandle;

			Effector->FinishSpawning(SpawnTransform);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTGameplayAbility_BlackHole::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

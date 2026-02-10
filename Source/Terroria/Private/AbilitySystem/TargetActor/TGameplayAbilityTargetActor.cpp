// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TargetActor/TGameplayAbilityTargetActor.h"

#include "Abilities/GameplayAbility.h"

ATGameplayAbilityTargetActor::ATGameplayAbilityTargetActor()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceRange = 1500.0f;
	bDestroyOnConfirmation = true;
	ShouldProduceTargetDataOnServer = true;
	MyReticleActor = nullptr;
}

void ATGameplayAbilityTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!PrimaryPC)
	{
		return;
	}

	TraceUnderCursor(TraceHitResults);

	if (MyReticleActor && PrimaryPC->IsLocalController())
	{
		if (TraceHitResults.bBlockingHit)
		{
			MyReticleActor->SetActorLocation(TraceHitResults.ImpactPoint, false, nullptr, ETeleportType::None);
		}
		else
		{
			MyReticleActor->SetActorLocation(TraceHitResults.TraceEnd, false, nullptr, ETeleportType::None);
		}
	}
	else
	{
		MyReticleActor->SetActorHiddenInGame(true);
	}
}

bool ATGameplayAbilityTargetActor::TraceUnderCursor(FHitResult& TraceHitResult) const
{
	bool bHit = PrimaryPC->GetHitResultUnderCursor(
		ECC_Visibility,
		true,
		TraceHitResult
	);

	return bHit;
}

void ATGameplayAbilityTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;

	if (const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo())
	{
		PrimaryPC = ActorInfo->PlayerController.Get();
	}

	if (!PrimaryPC)
	{
		PrimaryPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	}

	MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
}

void ATGameplayAbilityTargetActor::ConfirmTargetingAndContinue()
{
	FHitResult HitResult = TraceHitResults;

	FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromHitResult(
		OwningAbility, HitResult);
	if (TargetData != nullptr)
	{
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}

	DestroyReticleActors();
}

void ATGameplayAbilityTargetActor::CancelTargeting()
{
	Super::CancelTargeting();
	DestroyReticleActors();
}

AGameplayAbilityWorldReticle* ATGameplayAbilityTargetActor::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	//Check if Reticle Class is not empty
	if (ReticleClass)
	{
		//Spawn only of MyReticleActor is empty
		if (!MyReticleActor)
		{
			AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(
				ReticleClass, Location, Rotation);
			//When we successfully spawn the actor
			if (SpawnedReticleActor)
			{
				SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
				//SpawnedReticleActor->SetReplicates(false);
				return SpawnedReticleActor;
			}
		}
		else
		{
			return MyReticleActor;
		}
	}
	return nullptr;
}

void ATGameplayAbilityTargetActor::DestroyReticleActors()
{
	if (MyReticleActor)
	{
		MyReticleActor->Destroy();
	}
}

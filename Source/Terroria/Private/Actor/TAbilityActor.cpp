// Copyright 2025 @xerlock. All Rights Reserved.


#include "Actor/TAbilityActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

ATAbilityActor::ATAbilityActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void ATAbilityActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApply)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectToApply, 1.f, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	if (EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite &&
		RemovalPolicy == EEffectRemovalPolicy::OnEndOverlaps)
	{
		ActiveGameplayEffectHandles.Add(ActiveGameplayEffectHandle, TargetASC);
	}
}

void ATAbilityActor::RemoveEffectFromTarget(AActor* TargetActor)
{
	if (ActiveGameplayEffectHandles.IsEmpty()) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC)) return;

	TArray<FActiveGameplayEffectHandle> RemoveToHandle;
	for (TPair Handles : ActiveGameplayEffectHandles)
	{
		if (TargetASC == Handles.Value)
		{
			TargetASC->RemoveActiveGameplayEffect(Handles.Key, 1);
			RemoveToHandle.Add(Handles.Key);
		}
	}

	for (FActiveGameplayEffectHandle& Handle : RemoveToHandle)
	{
		ActiveGameplayEffectHandles.FindAndRemoveChecked(Handle);
	}
	
}

void ATAbilityActor::OnBeginOverlap(AActor* OverlappedActor)
{
	if (ApplyPolicy == EEffectApplyPolicy::OnBeginOverlaps)
	{
		ApplyEffectToTarget(OverlappedActor, GameplayEffectClass);
	}
}

void ATAbilityActor::OnEndOverlap(AActor* OverlappedActor)
{
	if (ApplyPolicy == EEffectApplyPolicy::OnEndOverlaps)
	{
		ApplyEffectToTarget(OverlappedActor, GameplayEffectClass);
	}

	if (RemovalPolicy == EEffectRemovalPolicy::OnEndOverlaps)
	{
		RemoveEffectFromTarget(OverlappedActor);
	}
}



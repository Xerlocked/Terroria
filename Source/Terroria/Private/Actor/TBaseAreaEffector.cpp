// Copyright 2025 @xerlock. All Rights Reserved.


#include "Actor/TBaseAreaEffector.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
ATBaseAreaEffector::ATBaseAreaEffector()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

// Called when the game starts or when spawned
void ATBaseAreaEffector::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetLifeSpan(Duration);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATBaseAreaEffector::ApplyEffectToTarget, TickInterval,
		                                true);
	}
}

void ATBaseAreaEffector::ApplyEffectToTarget()
{
	if (!OverlapComponent || !EffectSpecHandle.IsValid())
	{
		return;
	}

	TArray<AActor*> OverlappedActors;
	OverlapComponent->GetOverlappingActors(OverlappedActors);

	for (AActor* Target : OverlappedActors)
	{
		if (Target == GetInstigator())
		{
			continue;
		}

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

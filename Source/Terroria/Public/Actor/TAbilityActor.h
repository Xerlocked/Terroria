// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "TAbilityActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM()
enum class EEffectApplyPolicy
{
	DoNothing,
	OnBeginOverlaps,
	OnEndOverlaps,
};

UENUM()
enum class EEffectRemovalPolicy
{
	DoNothing,
	OnEndOverlaps,
};

UCLASS()
class TERRORIA_API ATAbilityActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATAbilityActor();


protected:

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApply);

	UFUNCTION(BlueprintCallable)
	void RemoveEffectFromTarget(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* OverlappedActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* OverlappedActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effect")
	EEffectApplyPolicy ApplyPolicy = EEffectApplyPolicy::DoNothing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effect")
	EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNothing;
	
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGameplayEffectHandles;
};

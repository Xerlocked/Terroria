// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TAbilityActor.generated.h"

class UGameplayEffect;

UCLASS()
class TERRORIA_API ATAbilityActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATAbilityActor();


protected:

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApply);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

};

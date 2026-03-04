// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCueNotify_Trail.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API AGameplayCueNotify_Trail : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};

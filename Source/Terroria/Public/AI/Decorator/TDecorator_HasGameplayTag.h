// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TDecorator_HasGameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UTDecorator_HasGameplayTag : public UBTDecorator
{
	GENERATED_BODY()

public:
	UTDecorator_HasGameplayTag();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Cond")
	FGameplayTag GameplayTag;
};

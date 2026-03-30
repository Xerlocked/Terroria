// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckReturnState.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UBTService_CheckReturnState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckReturnState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector HomeLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector bNeedReturnKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector bReachedHomeKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LeashRadiusKey;

	UPROPERTY(EditAnywhere, Category = "Leash", meta = (ClampMin = "0.0"))
	float HomeReachedThreshold = 100.f;
};

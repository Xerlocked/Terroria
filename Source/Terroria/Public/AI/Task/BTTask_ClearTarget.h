// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearTarget.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UBTTask_ClearTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ClearTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LastKnownTargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector bInAttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector bNeedReturnKey;

	/** If true, also reset State tag to Idle after clearing */
	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bResetToIdle = true;
};

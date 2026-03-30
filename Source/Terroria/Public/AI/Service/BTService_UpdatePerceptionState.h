// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdatePerceptionState.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UBTService_UpdatePerceptionState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdatePerceptionState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector bHasValidTargetKey;
};

// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TAIController.generated.h"

struct FAIStimulus;

struct FAIKey
{
	// Object
	static const FName TargetActor;

	// Vector
	static const FName SpawnLocation;
	static const FName HomeLocation;
	static const FName LastKnownTargetLocation;
	static const FName MoveTargetLocation;
	static const FName PatrolPoint;
	static const FName RangedAttackPosition;

	// Bool
	static const FName bHasLOS;
	static const FName bInAttackRange;
	static const FName bInDetectRange;
	static const FName bNeedReturn;
	static const FName bReachedHome;
	static const FName bHasValidTarget;
	static const FName bShouldSearch;
	static const FName bCanWander;
	static const FName bShouldKeepDistance;

	// Float
	static const FName DistanceToTarget;
	static const FName SearchElapsedTime;
	static const FName AttackCooldownRemaining;
	static const FName TargetLocationScore;
	static const FName AttackRange;
	static const FName LeashRadius;
};

/**
 * 
 */
UCLASS()
class TERRORIA_API ATAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATAIController();

	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BlackboardData;

private:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};

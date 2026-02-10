// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TAIController.generated.h"

struct FAIStimulus;

struct FAIKey
{
	static const FName TargetActor;
	static const FName AttackRange;
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

// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetStateTag.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UBTTask_SetStateTag : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetStateTag();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "GameplayTag")
	FGameplayTag NewStateTag;

	UPROPERTY(EditAnywhere, Category = "GameplayTag")
	FGameplayTag StateParentTag;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CurrentStateTagKey;
};

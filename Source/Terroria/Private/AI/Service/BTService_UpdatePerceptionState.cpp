// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Service/BTService_UpdatePerceptionState.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdatePerceptionState::UBTService_UpdatePerceptionState()
{
	NodeName = "Update Perception State";
	Interval = 0.25f;
	RandomDeviation = 0.05f;
}

void UBTService_UpdatePerceptionState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	const bool bHasValidTarget = IsValid(TargetActor);
	BB->SetValueAsBool(bHasValidTargetKey.SelectedKeyName, bHasValidTarget);
}

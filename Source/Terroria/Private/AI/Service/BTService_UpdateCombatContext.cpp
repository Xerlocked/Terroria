// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Service/BTService_UpdateCombatContext.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateCombatContext::UBTService_UpdateCombatContext()
{
	NodeName = "Update Combat Context";
	Interval = 0.25;
	RandomDeviation = 0.05f;
}

void UBTService_UpdateCombatContext::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!BB || !AIC)
	{
		return;
	}

	APawn* ControlledPawn = AIC->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));

	const float AttackRange = BB->GetValueAsFloat(AttackRangeKey.SelectedKeyName);

	// Target validity
	const bool bIsValidTarget = IsValid(TargetActor) && !TargetActor->IsActorBeingDestroyed();
	BB->SetValueAsBool(bHasValidTargetKey.SelectedKeyName, bIsValidTarget);

	if (!bIsValidTarget)
	{
		BB->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, 0.f);
		BB->SetValueAsBool(bInAttackRangeKey.SelectedKeyName, false);
		BB->SetValueAsBool(bInDetectRangeKey.SelectedKeyName, false);
		BB->SetValueAsBool(bHasLOSKey.SelectedKeyName, false);
		return;
	}

	// Distance
	const float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	BB->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);

	// Attack range
	BB->SetValueAsBool(bInAttackRangeKey.SelectedKeyName, Distance <= AttackRange);

	// Detect range
	BB->SetValueAsBool(bInDetectRangeKey.SelectedKeyName, Distance <= DetectRange);

	BB->SetValueAsBool(bHasLOSKey.SelectedKeyName, true);
}

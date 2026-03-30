// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Service/BTService_CheckReturnState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckReturnState::UBTService_CheckReturnState()
{
	NodeName = "Check Return State";
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_CheckReturnState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	const FVector HomeLocation = BB->GetValueAsVector(HomeLocationKey.SelectedKeyName);
	const FVector CurrentLocation = ControlledPawn->GetActorLocation();
	const float DistFromHome = FVector::Dist(CurrentLocation, HomeLocation);
	const float LeashRadius = BB->GetValueAsFloat(LeashRadiusKey.SelectedKeyName);

	// Leash exceeded -> must return
	const bool bLeashExceeded = DistFromHome > LeashRadius;

	// If already needing return, keep it true until we reach home
	const bool bCurrentlyNeedReturn = BB->GetValueAsBool(bNeedReturnKey.SelectedKeyName);

	if (bLeashExceeded && !bCurrentlyNeedReturn)
	{
		BB->SetValueAsBool(bNeedReturnKey.SelectedKeyName, true);
	}

	// Reached home check
	const bool bReachedHome = DistFromHome <= HomeReachedThreshold;
	BB->SetValueAsBool(bReachedHomeKey.SelectedKeyName, bReachedHome);

	// If reached home, clear the return flag
	if (bReachedHome && bCurrentlyNeedReturn)
	{
		BB->SetValueAsBool(bNeedReturnKey.SelectedKeyName, false);
	}
}

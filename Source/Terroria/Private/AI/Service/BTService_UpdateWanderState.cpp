// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Service/BTService_UpdateWanderState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "TGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateWanderState::UBTService_UpdateWanderState()
{
	NodeName = "Update Wander State";
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_UpdateWanderState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!BB || !AIC)
	{
		return;
	}

	const bool bHasTarget = BB->GetValueAsBool(bHasValidTargetKey.SelectedKeyName);
	const bool bNeedReturn = BB->GetValueAsBool(bNeedReturnKey.SelectedKeyName);

	// Check status tags
	bool bIsDisabled = false;
	APawn* Pawn = AIC->GetPawn();
	if (Pawn)
	{
		const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn);
		if (ASI)
		{
			const UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
			if (ASC)
			{
				bIsDisabled = ASC->HasMatchingGameplayTag(FTGameplayTags::Get().State_Stunned)
					|| ASC->HasMatchingGameplayTag(FTGameplayTags::Get().State_Death);
			}
		}
	}

	const bool bCanWander = !bHasTarget && !bNeedReturn && !bIsDisabled;
	BB->SetValueAsBool(bCanWanderKey.SelectedKeyName, bCanWander);
}

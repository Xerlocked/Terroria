// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Task/BTTask_ClearTarget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "TGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearTarget::UBTTask_ClearTarget()
{
	NodeName = "Clear Target";
}

EBTNodeResult::Type UBTTask_ClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	// Clear target actor
	BB->ClearValue(TargetActorKey.SelectedKeyName);

	// Clear last known location
	BB->ClearValue(LastKnownTargetLocationKey.SelectedKeyName);

	// Reset flags
	BB->SetValueAsBool(bInAttackRangeKey.SelectedKeyName, false);

	// Optionally reset state tag to Idle
	if (bResetToIdle)
	{
		if (const AAIController* AIC = OwnerComp.GetAIOwner())
		{
			if (APawn* Pawn = AIC->GetPawn())
			{
				if (const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn))
				{
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
					{
						// Remove all State.* tags
						const FGameplayTag StateParent = FGameplayTag::RequestGameplayTag(FName("State"));
						FGameplayTagContainer OwnedTags;
						ASC->GetOwnedGameplayTags(OwnedTags);
						for (const FGameplayTag& Tag : OwnedTags)
						{
							if (Tag.MatchesTag(StateParent))
							{
								ASC->RemoveLooseGameplayTag(Tag);
							}
						}
						ASC->AddLooseGameplayTag(FTGameplayTags::Get().State_Idle);
					}
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}

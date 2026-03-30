// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Task/BTTask_SetStateTag.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetStateTag::UBTTask_SetStateTag()
{
	NodeName = "Set State Tag";
	StateParentTag = FGameplayTag::RequestGameplayTag(FName("State"));
}

EBTNodeResult::Type UBTTask_SetStateTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Type::Failed;
	}

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Type::Failed;
	}

	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn);
	if (!ASI)
	{
		return EBTNodeResult::Type::Failed;
	}

	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC)
	{
		return EBTNodeResult::Type::Failed;
	}

	FGameplayTagContainer OwnedTags;
	ASC->GetOwnedGameplayTags(OwnedTags);

	for (const FGameplayTag& Tag : OwnedTags)
	{
		if (Tag.MatchesTag(StateParentTag))
		{
			ASC->RemoveLooseGameplayTag(Tag);
		}
	}

	if (NewStateTag.IsValid())
	{
		ASC->AddLooseGameplayTag(NewStateTag);
	}

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsName(CurrentStateTagKey.SelectedKeyName, NewStateTag.GetTagName());
	}

	return EBTNodeResult::Type::Succeeded;
}

FString UBTTask_SetStateTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("Set State: %s"), *NewStateTag.ToString());
}

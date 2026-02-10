// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/TAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TEnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"

const FName FAIKey::TargetActor(TEXT("TargetActor"));
const FName FAIKey::AttackRange(TEXT("AttackRange"));

ATAIController::ATAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATAIController::OnPerceptionUpdated);
}

void ATAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* LocalBlackboardComponent = nullptr;
	if (UseBlackboard(BlackboardData, LocalBlackboardComponent))
	{
		if (const ATEnemyCharacter* EnemyCharacter = Cast<ATEnemyCharacter>(InPawn))
		{
			GetBlackboardComponent()->SetValueAsFloat(FAIKey::AttackRange, EnemyCharacter->PrimaryAttackRange);
		}

		RunBehaviorTree(BehaviorTree);
	}
}

void ATAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->ActorHasTag(FName("Player")) && Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(FAIKey::TargetActor, Actor);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(FAIKey::TargetActor);
	}
}

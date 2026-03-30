// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/TAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TEnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"

const FName FAIKey::TargetActor(TEXT("TargetActor"));
const FName FAIKey::SpawnLocation(TEXT("SpawnLocation"));
const FName FAIKey::HomeLocation(TEXT("HomeLocation"));
const FName FAIKey::LastKnownTargetLocation(TEXT("LastKnownTargetLocation"));
const FName FAIKey::MoveTargetLocation(TEXT("MoveTargetLocation"));
const FName FAIKey::PatrolPoint(TEXT("PatrolPoint"));
const FName FAIKey::RangedAttackPosition(TEXT("RangedAttackPosition"));
const FName FAIKey::bHasLOS(TEXT("bHasLOS"));
const FName FAIKey::bInAttackRange(TEXT("bInAttackRange"));
const FName FAIKey::bInDetectRange(TEXT("bInDetectRange"));
const FName FAIKey::bNeedReturn(TEXT("bNeedReturn"));
const FName FAIKey::bReachedHome(TEXT("bReachedHome"));
const FName FAIKey::bHasValidTarget(TEXT("bHasValidTarget"));
const FName FAIKey::bShouldSearch(TEXT("bShouldSearch"));
const FName FAIKey::bCanWander(TEXT("bCanWander"));
const FName FAIKey::bShouldKeepDistance(TEXT("bShouldKeepDistance"));
const FName FAIKey::DistanceToTarget(TEXT("DistanceToTarget"));
const FName FAIKey::SearchElapsedTime(TEXT("SearchElapsedTime"));
const FName FAIKey::AttackCooldownRemaining(TEXT("AttackCooldownRemaining"));
const FName FAIKey::TargetLocationScore(TEXT("TargetLocationScore"));
const FName FAIKey::AttackRange(TEXT("AttackRange"));
const FName FAIKey::LeashRadius(TEXT("LeashRadius"));

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
			UBlackboardComponent* BB = GetBlackboardComponent();
			BB->SetValueAsVector(FAIKey::SpawnLocation, EnemyCharacter->GetActorLocation());
			BB->SetValueAsVector(FAIKey::HomeLocation, EnemyCharacter->GetActorLocation());

			BB->SetValueAsBool(FAIKey::bNeedReturn, false);
			BB->SetValueAsBool(FAIKey::bReachedHome, true);
			BB->SetValueAsBool(FAIKey::bHasValidTarget, false);
			BB->SetValueAsBool(FAIKey::bHasLOS, false);
			BB->SetValueAsBool(FAIKey::bInDetectRange, false);
			BB->SetValueAsBool(FAIKey::bInAttackRange, false);
			BB->SetValueAsBool(FAIKey::bShouldSearch, false);
			BB->SetValueAsBool(FAIKey::bCanWander, true);

			BB->SetValueAsFloat(FAIKey::SearchElapsedTime, 0.0f);
			BB->SetValueAsFloat(FAIKey::AttackCooldownRemaining, 0.0f);

			BB->SetValueAsFloat(FAIKey::AttackRange, EnemyCharacter->AttackRange);
			BB->SetValueAsFloat(FAIKey::LeashRadius, EnemyCharacter->LeashRadius);
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
}

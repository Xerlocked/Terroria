// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Task/BTTask_ActivateAbilityByTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ActivateAbilityByTag::UBTTask_ActivateAbilityByTag()
{
	NodeName = "Activate Ability By Tag";
	TargetActorKeySelector.AddObjectFilter(
		this, GET_MEMBER_NAME_CHECKED(UBTTask_ActivateAbilityByTag, TargetActorKeySelector), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlPawn)
	{
		return EBTNodeResult::Type::Failed;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlPawn);
	if (!ASC)
	{
		return EBTNodeResult::Type::Failed;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	AActor* TargetActor = nullptr;

	if (BlackboardComponent)
	{
		UObject* TargetObj = BlackboardComponent->GetValueAsObject(TargetActorKeySelector.SelectedKeyName);
		TargetActor = Cast<AActor>(TargetObj);
	}

	FGameplayEventData Payload;
	Payload.Instigator = ControlPawn;
	Payload.Target = TargetActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ControlPawn, GameplayTag, Payload);

	return EBTNodeResult::Type::Succeeded;
}

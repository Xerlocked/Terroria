// Copyright 2025 @xerlock. All Rights Reserved.


#include "AI/Decorator/TDecorator_HasGameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UTDecorator_HasGameplayTag::UTDecorator_HasGameplayTag()
{
	NodeName = "Has GameplayTag";
}

bool UTDecorator_HasGameplayTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlPawn)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlPawn);
	if (!ASC)
	{
		return false;
	}

	return ASC->HasMatchingGameplayTag(GameplayTag);
}

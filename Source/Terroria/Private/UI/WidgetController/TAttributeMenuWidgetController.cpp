// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TAttributeMenuWidgetController.h"

#include "TGameplayTags.h"
#include "AbilitySystem/TAttributeSet.h"
#include "AbilitySystem/Data/TAttributeDataAsset.h"

void UTAttributeMenuWidgetController::BroadcastInitialValues()
{
	UTAttributeSet* AS = CastChecked<UTAttributeSet>(AttributeSet);
	check(AttributeDataAsset);

	for (auto& Pair : AS->TagsToAttributes)
	{
		FTAttributeData AttributeData = AttributeDataAsset->FindAttributeDataByTag(Pair.Key);
		AttributeData.AttributeValue = Pair.Value().GetNumericValue(AS);
		OnAttributeDataDelegate.Broadcast(AttributeData);
	}
}

void UTAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}

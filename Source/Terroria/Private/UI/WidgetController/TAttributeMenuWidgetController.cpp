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
		BroadcastOnAttributeData(Pair.Key, Pair.Value());
	}
}

void UTAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UTAttributeSet* AS = CastChecked<UTAttributeSet>(AttributeSet);
	check(AttributeDataAsset);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastOnAttributeData(Pair.Key, Pair.Value());
			}
		);
	}
}

void UTAttributeMenuWidgetController::BroadcastOnAttributeData(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	FTAttributeData AttributeData = AttributeDataAsset->FindAttributeDataByTag(Tag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeDataDelegate.Broadcast(AttributeData);
}

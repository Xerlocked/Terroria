// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TAttributeMenuWidgetController.h"

#include "TGameplayTags.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"
#include "AbilitySystem/Data/TAttributeDataAsset.h"
#include "Player/TPlayerState.h"

void UTAttributeMenuWidgetController::BroadcastInitialValues()
{
	UTAttributeSet* AS = CastChecked<UTAttributeSet>(AttributeSet);
	check(AttributeDataAsset);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastOnAttributeData(Pair.Key, Pair.Value());
	}

	ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);
	AttributeChangedDelegate.Broadcast(TPlayerState->GetAttributePoint());
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

	ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);
	TPlayerState->OnAttributePointChangedDelegate.AddLambda([this](int32 Points)
	{
		AttributeChangedDelegate.Broadcast(Points);
	});
}

void UTAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UTAbilitySystemComponent* ASC = CastChecked<UTAbilitySystemComponent>(AbilitySystemComponent);
	ASC->UpgradeAttribute(AttributeTag);
}

void UTAttributeMenuWidgetController::BroadcastOnAttributeData(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	FTAttributeData AttributeData = AttributeDataAsset->FindAttributeDataByTag(Tag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeDataDelegate.Broadcast(AttributeData);
}

// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TOverlayWidgetController.h"

#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"

void UTOverlayWidgetController::BroadcastInitialValues()
{
	const UTAttributeSet* Attributes = CastChecked<UTAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(Attributes->GetHealth());
	OnMaxHealthChanged.Broadcast(Attributes->GetMaxHealth());
	OnManaChanged.Broadcast(Attributes->GetMana());
	OnMaxManaChanged.Broadcast(Attributes->GetMaxMana());
}

void UTOverlayWidgetController::BindCallbacksToDependencies()
{
	const UTAttributeSet* Attributes = CastChecked<UTAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});

	check(WidgetMessageDataTable);
	Cast<UTAbilitySystemComponent>(AbilitySystemComponent)->AssetTagsEvent.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			for (const FGameplayTag& AssetTag : AssetTags)
			{
				if (AssetTag.MatchesTag(MessageTag))
				{
					const FWidgetMessageRow* Row = GetDataTableRowByTag<FWidgetMessageRow>(WidgetMessageDataTable, AssetTag);
					OnAssetTagMessage.Broadcast(*Row);
				}
			}
		}
	);
}

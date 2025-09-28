// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TOverlayWidgetController.h"

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

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetHealthAttribute()).AddUObject(this, &UTOverlayWidgetController::CallbackHealth);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetMaxHealthAttribute()).AddUObject(this, &UTOverlayWidgetController::CallbackMaxHealth);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetManaAttribute()).AddUObject(this, &UTOverlayWidgetController::CallbackMana);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetMaxManaAttribute()).AddUObject(this, &UTOverlayWidgetController::CallbackMaxMana);
}

void UTOverlayWidgetController::CallbackHealth(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UTOverlayWidgetController::CallbackMaxHealth(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UTOverlayWidgetController::CallbackMana(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UTOverlayWidgetController::CallbackMaxMana(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TUserWidgetController.h"

void UTUserWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UTUserWidgetController::BroadcastInitialValues()
{
	
}

void UTUserWidgetController::BindCallbacksToDependencies()
{
	
}

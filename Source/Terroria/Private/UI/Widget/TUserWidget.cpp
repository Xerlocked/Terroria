// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/Widget/TUserWidget.h"

void UTUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	BindWidgetController();
}

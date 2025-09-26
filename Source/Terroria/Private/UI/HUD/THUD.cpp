// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/HUD/THUD.h"

#include "Blueprint/UserWidget.h"

void ATHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}

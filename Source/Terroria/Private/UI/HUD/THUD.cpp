// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/HUD/THUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/TUserWidget.h"
#include "UI/WidgetController/TOverlayWidgetController.h"

UTOverlayWidgetController* ATHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UTOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	
	return OverlayWidgetController;
}

void ATHUD::InitOverlayHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	checkf(OverlayWidgetClass, TEXT("OverlayWidget not initialized"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetController not initialized"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UTUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UTOverlayWidgetController* OWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(OWidgetController);
	OWidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

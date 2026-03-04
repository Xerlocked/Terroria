// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/HUD/THUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/TUserWidget.h"
#include "UI/WidgetController/TAttributeMenuWidgetController.h"
#include "UI/WidgetController/TOverlayWidgetController.h"
#include "UI/WidgetController/TShopWidgetController.h"

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

UTAttributeMenuWidgetController* ATHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UTAttributeMenuWidgetController>(
			this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

UTShopWidgetController* ATHUD::GetShopWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (ShopWidgetController == nullptr)
	{
		ShopWidgetController = NewObject<UTShopWidgetController>(this, ShopWidgetControllerClass);
		ShopWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		ShopWidgetController->BindCallbacksToDependencies();
	}

	return ShopWidgetController;
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

void ATHUD::InitShopHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(ShopWidgetClass, TEXT("ShopWidget not initialized"));
	checkf(ShopWidgetControllerClass, TEXT("ShopWidgetController not initialized"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
	ShopWidget = Cast<UTUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UTShopWidgetController* SWidgetController = GetShopWidgetController(WidgetControllerParams);

	ShopWidget->SetWidgetController(SWidgetController);
	SWidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Hidden);
}

void ATHUD::ToggleShopWidget() const
{
	if (!ShopWidget)
	{
		return;
	}

	if (ShopWidget->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ShopWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
